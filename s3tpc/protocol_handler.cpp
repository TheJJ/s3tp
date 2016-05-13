#include "protocol_handler.h"

#include <iostream>
#include <unistd.h>
#include "connection.h"
#include "network_event.h"
#include "new_connection_event.h"
#include "opcodes.h"
#include "s3tpc.h"


namespace s3tpc {


ProtocolHandler::ProtocolHandler(S3TPClient *parent)
	:
	parent{parent},
	control_socket{parent->get_control_socket()},
	buffer{ProtocolHandler::BUFFER_SIZE, true, ProtocolHandler::RECEIVE_SIZE},
	event_id_counter{0},
	current_opcode{-1},
	current_event_id{-1} {
}


std::shared_ptr<NetworkEvent> ProtocolHandler::register_new_connection(const std::shared_ptr<Connection> &connection) {
	auto event = std::make_shared<NewConnectionEvent>(connection, this->event_id_counter++);
	std::unique_lock<std::mutex> lock{this->events_mutex};
	this->events.insert({event->get_id(), event});
	return event;
}


void ProtocolHandler::dispatch_incoming_data() {
	this->buffer.recv(this->control_socket);

	if (!this->read_opcode_and_event_id()) {
		return;
	}

	std::unique_lock<std::mutex> lock{this->events_mutex};
	auto connection_it = this->events.find(this->current_event_id);
	if (connection_it == std::end(this->events)) {
		// TODO illegal response, throw exception or something
		// does throw really unlock the lock?
		return;
	}

	bool processed = connection_it->second->handle_response(this->current_opcode, this->buffer);
	if (processed) {
		this->events.erase(connection_it);
	}
	lock.unlock();
}


bool ProtocolHandler::read_opcode_and_event_id() {
	if (this->current_opcode >= 0 && this->current_event_id >= 0) {
		return true;
	}

	if (!this->buffer.is_data_available(6)) {
		return false;
	}

	current_opcode = this->buffer.read_uint16();
	current_event_id = this->buffer.read_uint32();
	return true;
}


}
