#include "s3tpc.h"

#include <memory>
#include <system_error>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include "new_connection_event.h"


namespace s3tpc {


S3TPClient::S3TPClient()
	:
	control_socket{this->init_socket()},
	dispatcher{this} {
}


S3TPClient::~S3TPClient() {
	close(this->control_socket);
}


void S3TPClient::connect(const std::string &socket_path) {
	struct sockaddr_un addr;
	addr.sun_family = AF_UNIX;
	strncpy(addr.sun_path, socket_path.c_str(), sizeof(addr.sun_path));

	int len = strlen(addr.sun_path) + sizeof(addr.sun_family);
	int ret = ::connect(this->control_socket, (struct sockaddr *)&addr, len);
	if (ret != 0) {
		throw std::system_error(errno, std::system_category());
	}
}


void S3TPClient::start() {
	this->dispatcher.start();
}


void S3TPClient::stop() {
	this->dispatcher.stop();
	// TODO proper closing
	close(this->control_socket);
}


int S3TPClient::get_control_socket() const {
	return this->control_socket;
}


std::shared_ptr<Connection> S3TPClient::create_connection() {
	auto connection = std::make_shared<Connection>(this);
	auto event = std::make_shared<NewConnectionEvent>(connection);
	this->dispatcher.push_event(event);
	event->wait_for_resolution();
	return connection;
}


void S3TPClient::dispatch_incoming_data() {
}


int S3TPClient::init_socket() {
	int sock = socket(AF_UNIX, SOCK_STREAM, 0);
	if (sock < 0) {
		throw std::system_error(errno, std::system_category());
	}
	this->unblock_socket(sock);
	return sock;
}


void S3TPClient::unblock_socket(int sock) {
	int ret;
	int flags = fcntl(sock, F_GETFL, 0);
	if (flags < 0) {
		throw std::system_error(errno, std::system_category());
	}
	flags |= O_NONBLOCK;
	ret = fcntl(sock, F_SETFL, flags);
	if (ret != 0) {
		throw std::system_error(errno, std::system_category());
	}
}


std::unique_ptr<S3TPClient> S3TPClient::instance;


S3TPClient &S3TPClient::get_instance() {
	if (!S3TPClient::instance) {
		S3TPClient::instance = std::make_unique<S3TPClient>();
	}
	return *S3TPClient::instance;
}


}
