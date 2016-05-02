#include "dispatcher.h"

#include <sys/epoll.h>
#include <unistd.h>

#define MAX_EVENTS 64


int initialize_epoll_event(int epoll_fd, int fd, uint32_t events) {
	int ret;
	struct epoll_event event = {0};

	event.data.fd = fd;
	// set events to read and write
	// keep default level triggered behaviour
	event.events = events;
	if ((ret = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &event)) != 0) {
		// TODO proper error code
		return ret;
	}
	return 0;
}


int initialize_epoll(int control_socket, int notification_fd) {
	int epoll_fd;
	int ret;

	if ((epoll_fd = epoll_create1(0)) < 0) {
		// TODO proper error code
		return epoll_fd;
	}

	// initialize control socket event TODO really write
	if ((ret = initialize_epoll_event(epoll_fd, control_socket, EPOLLIN)) != 0) {
		// TODO proper error code
		return ret;
	}

	// initialize notification_fd
	if ((ret = initialize_epoll_event(epoll_fd, notification_fd, EPOLLIN)) != 0) {
		// TODO proper error code
		return ret;
	}

	return epoll_fd;
}


int was_woken_up(struct epoll_event *event) {
	uint64_t notification_value;
	if (event->data.fd != notification_fd) {
		return 0;
	}
	// TODO dont ignore return value although should not happen
	read(notification_fd, (void*)(&notification_value), sizeof(uint64_t));
	return notification_value;
}


int dispatch_event(struct epoll_event *event) {
	// read
	if (event->events & EPOLLIN) {
	}
}


int dispatch_loop(int control_socket) {
	int epoll_fd;
	int event_count;
	int i;
	int ret;
	struct epoll_event *event;
	struct epoll_event events[MAX_EVENTS];

	if((epoll_fd = initialize_epoll(control_socket, notification_fd)) < 0) {
		// TODO proper error code
		return epoll_fd;
	}

	printf("notification_fd: %d\n", notification_fd);
	printf("control_socket: %d\n", control_socket);

	while (is_running) {
		event_count = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);

		printf("incoming events: %d\n", event_count);
		for (i = 0; i < event_count; i++) {
			event = &events[i];

			printf("event: %d\n", event->data.fd);
			if (was_woken_up(event)) {
				if (is_running == 0) {
					break;
				}
				// TODO process send queues or other events
			} else if ((event->events & EPOLLERR) || (event->events & EPOLLHUP)) {
				printf("There was an error\n");
				// TODO handle error or hangup (remote closed?)
			} else if ((ret = dispatch_event(event)) != 0) {
				printf("There was an error handling the given event\n");
			}
		}
	}
	return 0;
}


int notify_dispatcher() {
	uint64_t notification_value = 1UL;
	if (write(notification_fd, (void*)(&notification_value), sizeof(uint64_t)) < 0) {
		return -1;
	}
	return 0;
}
