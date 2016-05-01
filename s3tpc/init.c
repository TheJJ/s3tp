#include "init.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fcntl.h>
#include <pthread.h>
#include <sys/eventfd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include "dispatcher.h"
#include "util.h"


static pthread_t main_worker;
static int control_socket;
static int notification_fd;

int is_running;


void *dispatch_socket(UNUSED(void* user_param)) {
	int ret = dispatch_loop(control_socket, notification_fd);
	return NULL;
}


/** Makes the control socket a non-blocking one. */
int make_socket_non_blocking() {
	int flags;
	int ret;

	if((flags = fcntl(control_socket, F_GETFL, 0)) < 0) {
		return flags;
	}

	flags |= O_NONBLOCK;
	if((ret = fcntl(control_socket, F_SETFL, flags)) != 0) {
		return ret;
	}

	return 0;
}


int init_control_socket(const char *s3tpd_socket_path) {
	struct sockaddr_un s3tpd_addr;
	int len;
	int ret;

	control_socket = socket(AF_UNIX, SOCK_STREAM, 0);
	if (control_socket == -1) {
		// TODO proper error code
		return -1;
	}

	// TODO proper error code
	if ((ret = make_socket_non_blocking()) != 0) {
		return ret;
	}

	s3tpd_addr.sun_family = AF_UNIX;
	strncpy(s3tpd_addr.sun_path, s3tpd_socket_path, sizeof(s3tpd_addr.sun_path));
	len = strlen(s3tpd_addr.sun_path) + sizeof(s3tpd_addr.sun_family);

	if (connect(control_socket, (struct sockaddr *)&s3tpd_addr, len) != 0) {
		// TODO proper error code
		return -1;
	}

	return 0;
}


int s3tp_init() {
	return s3tp_init_with_socket(S3TPD_SOCKET_PATH);
}


int s3tp_init_with_socket(const char *s3tpd_socket_path) {
	int ret;
	if ((ret = init_control_socket(s3tpd_socket_path)) != 0) {
		return ret;
	}
	if((notification_fd = eventfd(0, EFD_NONBLOCK)) < 0) {
		// TODO proper error code
		return notification_fd;
	}
	is_running = 1;
	return pthread_create(&main_worker, NULL, dispatch_socket, NULL);
}


void s3tp_destroy() {
	is_running = 0;
	close(control_socket);
	close(notification_fd);
	pthread_join(main_worker, NULL);
}
