#include "protocol_handler.h"

#include <iostream>
#include <unistd.h>
#include "connection.h"
#include "new_connection_event.h"
#include "opcodes.h"
#include "s3tpc.h"


namespace s3tpc {


ProtocolHandler::ProtocolHandler(S3TPClient *parent)
	:
	parent{parent},
	control_socket{parent->get_control_socket()},
	buffer{ProtocolHandler::BUFFER_SIZE, true, ProtocolHandler::RECEIVE_SIZE},
	event_id_counter{0} {
}


std::shared_ptr<NewConnectionEvent> ProtocolHandler::register_new_connection(const std::shared_ptr<Connection> &connection) {
	auto event = std::make_shared<NewConnectionEvent>(connection, this->event_id_counter++);
	std::unique_lock<std::mutex> lock{this->new_connection_mutex};
	this->new_connection_requests.insert({event->get_id(), event});
	return event;
}


void ProtocolHandler::dispatch_incoming_data() {
	this->buffer.recv(this->control_socket);

	int opcode = this->read_message_opcode();
	if (opcode == -1) {
		return;
	}

	switch(opcode) {
	case NEW_CONNECTION_ACK:
		this->handle_new_connection(6, true);
		break;
	case NEW_CONNECTION_NACK:
		this->handle_new_connection(4, false);
		break;
	default:
		break;
	}
}


int ProtocolHandler::read_message_opcode() {
	if (!this->buffer.is_data_available(2)) {
		return -1;
	}
	return this->buffer.get_uint16();
}


void ProtocolHandler::handle_new_connection(size_t response_size, bool success) {
	if (!this->buffer.is_data_available(response_size, sizeof(uint16_t))) {
		return;
	}

	uint32_t event_id = this->buffer.get_uint32(2);

	std::unique_lock<std::mutex> lock{this->new_connection_mutex};
	auto connection_it = this->new_connection_requests.find(event_id);
	if (connection_it == std::end(this->new_connection_requests)) {
		// TODO illegal response, throw exception or something
		// does throw really unlock the lock?
	}
	this->new_connection_requests.erase(connection_it);
	lock.unlock();
	if (success) {
		uint16_t connection_id = this->buffer.get_uint16(6);
		connection_it->second->initialize_connection(connection_id);
	}
	this->buffer.discard(8);
}


}
