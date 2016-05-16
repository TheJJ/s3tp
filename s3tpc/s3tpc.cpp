#include "s3tpc.h"

#include <iostream>
#include <memory>
#include <system_error>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include "close_connection_event.h"
#include "connect_event.h"
#include "listen_event.h"
#include "new_connection_event.h"
#include "protocol_exception.h"


namespace s3tpc {


S3TPClient::S3TPClient()
	:
	control_socket{this->init_socket()},
	protocol_handler{this},
	dispatcher{this} {
}


S3TPClient::~S3TPClient() {
	close(this->control_socket);
}


void S3TPClient::connect_to_s3tpd(const std::string &socket_path) {
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
	auto event = this->protocol_handler.register_event<NewConnectionEvent>(connection);
	this->dispatcher.push_event(event);
	event->wait_for_resolution();
	if (connection->has_state(ConnectionState::NEW)) {
		return nullptr;
	}
	return connection;
}


bool S3TPClient::close_connection(uint16_t id) {
	return this->create_and_wait_for_connection_event<CloseConnectionEvent>(id);
}


bool S3TPClient::connect(uint16_t id, uint16_t port) {
	return this->create_and_wait_for_connection_event<ConnectEvent>(id, port);
}


bool S3TPClient::listen(uint16_t id, uint16_t port) {
	return this->create_and_wait_for_connection_event<ListenEvent>(id, port);
}


void S3TPClient::dispatch_incoming_data() {
	try {
		this->protocol_handler.dispatch_incoming_data();
	} catch(const ProtocolException &e) {
		this->protocol_handler.reset();
	}
}


void S3TPClient::register_connection(const std::shared_ptr<Connection> &connection) {
	std::unique_lock<std::mutex> lock{this->connections_mutex};
	this->connections.insert({connection->get_id(), connection});
}


void S3TPClient::deregister_connection(uint16_t id) {
	std::unique_lock<std::mutex> lock{this->connections_mutex};
	auto connection_it = this->connections.find(id);
	if (connection_it != std::end(this->connections)) {
		this->connections.erase(connection_it);
	}
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


std::shared_ptr<Connection> S3TPClient::get_connection(uint16_t id) {
	std::unique_lock<std::mutex> lock{this->connections_mutex};
	auto connection_it = this->connections.find(id);
	if (connection_it == std::end(this->connections)) {
		return nullptr;
	}
	return connection_it->second;
}


std::unique_ptr<S3TPClient> S3TPClient::instance;


S3TPClient &S3TPClient::get_instance() {
	if (!S3TPClient::instance) {
		S3TPClient::instance = std::make_unique<S3TPClient>();
	}
	return *S3TPClient::instance;
}


}
