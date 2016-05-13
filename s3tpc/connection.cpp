#include "connection.h"

#include "s3tpc.h"


namespace s3tpc {


constexpr const uint16_t Connection::NEW_CONNECTION_ID;


Connection::Connection(S3TPClient *parent)
	:
	parent{parent},
	id{Connection::NEW_CONNECTION_ID},
	initialized{false} {
}


Connection::~Connection() {
}


void Connection::initialize(uint16_t id, const std::shared_ptr<Connection> &tracked_self) {
	if (!this->initialized) {
		this->initialized = true;
		this->id = id;
		this->parent->register_connection(tracked_self);
	}
}


void Connection::close() {
	this->parent->deregister_connection(this->get_id());
}


int Connection::get_id() const {
	return this->id;
}


bool Connection::is_initialized() const {
	return this->initialized;
}


}
