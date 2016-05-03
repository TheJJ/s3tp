#include "dispatcher.h"


#include <cstring>
#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <unistd.h>

#include "s3tpc.h"


namespace s3tpc {


constexpr const uint64_t Dispatcher::NOTIFICATION_VALUE;


Dispatcher::Dispatcher(S3TPClient *parent)
		:
		parent{parent},
		notification_fd{this->init_notification_fd()},
		epoll_fd{this->init_epoll_fd()},
		is_running{false} {
}


Dispatcher::~Dispatcher() {
}


void Dispatcher::start() {
	this->is_running = true;
	auto worker = std::make_unique<std::thread>(&Dispatcher::main_loop, this);
	this->main_worker.swap(worker);
}


void Dispatcher::stop() {
	this->main_worker->join();
}


void Dispatcher::notify() {
	// TODO does this need synchronization
	ssize_t ret = write(this->notification_fd, (void*)(&Dispatcher::NOTIFICATION_VALUE), sizeof(uint64_t));
	if (ret < 0) {
		// error
	}
}


int Dispatcher::init_notification_fd() {
	int fd = eventfd(0, EFD_NONBLOCK);
	if (fd < 0) {
		// error
	}
	return fd;
}


int Dispatcher::init_epoll_fd() {
	int efd = epoll_create1(0);
	if (efd < 0) {
		// error
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
		// error
	}
}


void Dispatcher::main_loop() {
	while (this->is_running) {
	}
}


}
