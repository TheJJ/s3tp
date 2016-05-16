#include "protocol_handler.h"

#include <iostream>
#include <unistd.h>
#include "connection.h"
#include "network_event.h"
#include "new_connection_event.h"
#include "opcodes.h"
#include "protocol_exception.h"
#include "s3tpc.h"


namespace s3tpc {


ProtocolHandler::ProtocolHandler(S3TPClient *parent)
	:
	parent{parent},
	control_socket{parent->get_control_socket()},
	buffer{ProtocolHandler::BUFFER_SIZE, true, ProtocolHandler::RECEIVE_SIZE},
	event_id_counter{1},
	current_opcode{-1},
	current_event_id{-1} {
}


void ProtocolHandler::dispatch_incoming_data() {
	this->buffer.recv(this->control_socket);

	if (!this->read_opcode_and_event_id()) {
		return;
	}

	// TODO if current_event_id == 0 (NO_MATCHING_EVENT) then there is no corresponding event

	std::unique_lock<std::mutex> lock{this->events_mutex};
	auto event_it = this->events.find(this->current_event_id);
	if (event_it == std::end(this->events)) {
		throw ProtocolException{"No matching network event is registered."};
	}

	if (!event_it->second->supports_opcode(this->current_opcode)) {
		throw ProtocolException{"Unsupported opcode for processing event."};
	}

	bool processed = event_it->second->handle_response(this->current_opcode, this->buffer);
	if (processed) {
		this->events.erase(event_it);
		this->reset_opcode_and_event_id();
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

	this->current_opcode = this->buffer.read_uint16();
	this->current_event_id = this->buffer.read_uint32();
	return true;
}


void ProtocolHandler::reset_opcode_and_event_id() {
	this->current_opcode = -1;
	this->current_event_id = -1;
}


void ProtocolHandler::reset() {
	this->reset_opcode_and_event_id();
	this->buffer.clear();
}


}
