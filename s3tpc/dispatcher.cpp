#include "dispatcher.h"


#include <cstring>
#include <iostream>
#include <system_error>
#include <sys/eventfd.h>
#include <unistd.h>

#include "s3tpc.h"
#include "stop_event.h"


namespace s3tpc {


constexpr const size_t Dispatcher::MAX_EVENTS;

constexpr const uint64_t Dispatcher::NOTIFICATION_VALUE;


Dispatcher::Dispatcher(S3TPClient *parent)
		:
		parent{parent},
		notification_fd{this->init_notification_fd()},
		epoll_fd{this->init_epoll_fd()},
		is_running{false} {
}


void Dispatcher::start() {
	this->is_running = true;
	auto worker = std::make_unique<std::thread>(&Dispatcher::main_loop, this);
	this->main_worker.swap(worker);
}


void Dispatcher::stop() {
	auto stop_event = std::make_shared<StopEvent>([this]() {
		this->is_running = false;
	});
	this->push_event(stop_event);
	this->main_worker->join();
	// TODO proper closing
	close(this->notification_fd);
}


void Dispatcher::push_event(const std::shared_ptr<Event> &event) {
	std::unique_lock<std::mutex> lock{this->event_mutex};
	this->pending_events.push(event);
	this->notify();
}


int Dispatcher::get_control_socket() const {
	return this->parent->get_control_socket();
}


void Dispatcher::notify() {
	// TODO does this need synchronization
	ssize_t ret = write(this->notification_fd, (void*)(&Dispatcher::NOTIFICATION_VALUE), sizeof(uint64_t));
	if (ret < 0) {
		throw std::system_error(errno, std::system_category());
	}
}


int Dispatcher::init_notification_fd() {
	int fd = eventfd(0, EFD_NONBLOCK);
	if (fd < 0) {
		throw std::system_error(errno, std::system_category());
	}
	return fd;
}


int Dispatcher::init_epoll_fd() {
	int efd = epoll_create1(0);
	if (efd < 0) {
		throw std::system_error(errno, std::system_category());
	}

	this->init_epoll_event(efd, this->parent->get_control_socket(), EPOLLIN);
	this->init_epoll_event(efd, this->notification_fd, EPOLLIN);

	return efd;
}


void Dispatcher::init_epoll_event(int epoll_fd, int fd, uint32_t events) {
	struct epoll_event event;
	memset(&event, 0, sizeof(event));
	event.data.fd = fd;
	event.events = events;
	int ret = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &event);
	if (ret != 0) {
		throw std::system_error(errno, std::system_category());
	}
}


void Dispatcher::main_loop() {
	struct epoll_event events[Dispatcher::MAX_EVENTS];
	struct epoll_event *event;

	while (this->is_running) {
		int event_count = epoll_wait(this->epoll_fd, events, Dispatcher::MAX_EVENTS, -1);

		if (event_count == -1) {
			// TODO handle error in thread
		}

		for (int i = 0; i < event_count; i++) {
			event = &events[i];

			if ((event->events & EPOLLERR) || (event->events & EPOLLHUP)) {
				// TODO handle error on remote closed or other stuff
			} else {
				uint64_t notification_count = this->get_notification_count(event);
				if (notification_count > 0) {
					this->dispatch_queue(notification_count);
					if (!this->is_running) {
						break;
					}
				} else if ((event->events & EPOLLIN) && event->data.fd == this->get_control_socket()){
					this->parent->dispatch_incoming_data();
				}
			}
		}
	}
}


uint64_t Dispatcher::get_notification_count(struct epoll_event *event) {
	uint64_t notification_count;
	if (event->data.fd != this->notification_fd) {
		return false;
	}
	read(this->notification_fd, (void*)(&notification_count), sizeof(uint64_t));
	return notification_count;
}


void Dispatcher::dispatch_queue(uint64_t event_count) {
	while (event_count > 0) {
		std::unique_lock<std::mutex> lock{this->event_mutex};
		auto event = this->pending_events.front();
		this->pending_events.pop();
		lock.unlock();
		event->dispatch(this);
		event_count--;
	}
}


}
