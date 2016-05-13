#include "new_connection_event.h"

#include <iostream>
#include <sys/socket.h>
#include "dispatcher.h"
#include "opcodes.h"
#include "protocol_exception.h"


namespace s3tpc {


NewConnectionEvent::NewConnectionEvent(const std::shared_ptr<Connection> &connection, uint32_t id)
	:
	NetworkEvent{id},
	connection{connection} {
}


void NewConnectionEvent::dispatch(Dispatcher *dispatcher) {
	int control_socket = dispatcher->get_control_socket();

	uint16_t opcode = NEW_CONNECTION_REQUEST;
	uint32_t event_id = this->get_id();
	send(control_socket, &opcode, 2, 0);
	send(control_socket, &event_id, 4, 0);
}


bool NewConnectionEvent::handle_response(uint16_t opcode, RingBuffer &buffer) {
	switch(opcode) {
	case NEW_CONNECTION_ACK:
		return this->handle_new_connection(buffer);
	case NEW_CONNECTION_NACK:
		this->resolve();
		return true;
	default:
		throw ProtocolException{"Illegal opcode for new connection event."};
	}
}


bool NewConnectionEvent::handle_new_connection(RingBuffer &buffer) {
	if (!buffer.is_data_available(2)) {
		return false;
	}

	uint16_t connection_id = buffer.read_uint16();
	this->connection->initialize(connection_id, this->connection);
	this->resolve();
	return true;
}


}
