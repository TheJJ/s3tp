#include "close_connection_event.h"

#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include "dispatcher.h"
#include "opcodes.h"


namespace s3tpc {


CloseConnectionEvent::CloseConnectionEvent(const std::shared_ptr<Connection> &connection, uint32_t id)
	:
	NetworkEvent{connection, id} {
}


void CloseConnectionEvent::dispatch(Dispatcher *dispatcher) {
	int control_socket = dispatcher->get_control_socket();
	char buffer[8];
	uint16_t connection_id = this->get_connection()->get_id();

	this->init_header(buffer, CLOSE_CONNECTION_REQUEST);
	memcpy(buffer+6, &connection_id, 4);
	send(control_socket, buffer, sizeof(buffer), 0);
	// TODO handle error
}


bool CloseConnectionEvent::handle_response(uint16_t opcode, RingBuffer &buffer) {
	switch(opcode) {
	case CLOSE_CONNECTION_ACK:
		this->get_connection()->close();
		this->resolve();
		return true;
	case CLOSE_CONNECTION_NACK:
		this->get_connection()->close();
		this->reject();
		return true;
	}
	return true;
}


bool CloseConnectionEvent::supports_opcode(uint16_t opcode) const {
	return opcode == CLOSE_CONNECTION_ACK || opcode == CLOSE_CONNECTION_NACK;
}


}
