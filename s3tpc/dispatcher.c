#include "dispatcher.h"

#include <sys/epoll.h>

#define MAX_EVENTS 64


int initialize_epoll(int control_socket) {
	int epoll_fd;
	int ret;
	struct epoll_event event = {0};

	if ((epoll_fd = epoll_create1(0)) < 0) {
		// TODO proper error code
		return epoll_fd;
	}

	event.data.fd = control_socket;
	// set events to read and write
	// keep default level triggered behaviour
	event.events = EPOLLIN | EPOLLOUT;
	if ((ret = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, control_socket, &event)) != 0) {
		// TODO proper error code
		return ret;
	}


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

	if((epoll_fd = initialize_epoll(control_socket)) != 0) {
		// TODO proper error code
		return epoll_fd;
	}

	while (is_running) {
		event_count = epoll_wait(epoll_fd, &events, MAX_EVENTS, -1);

		for (i = 0; i < event_count; i++) {
			event = &events[i];

			if ((event->events & EPOLLERR) || (event->events & EPOLLHUP)) {
				// TODO handle error or hangup (remote closed?)
			} else if ((ret = dispatch_event(event)) != 0) {
				// TODO handle error of dispatching
			}
		}
	}
	return 0;
}
