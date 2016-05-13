#include "new_connection_event.h"

#include <cstring>
#include <sys/socket.h>
#include "dispatcher.h"
#include "opcodes.h"


namespace s3tpc {


NewConnectionEvent::NewConnectionEvent(const std::shared_ptr<Connection> &connection, uint32_t id)
	:
	NetworkEvent{connection, id} {
}


void NewConnectionEvent::dispatch(Dispatcher *dispatcher) {
	int control_socket = dispatcher->get_control_socket();
	char buffer[6];

	uint16_t opcode = NEW_CONNECTION_REQUEST;
	uint32_t event_id = this->get_id();

	memcpy(buffer, &opcode, 2);
	memcpy(buffer+2, &event_id, 4);
	send(control_socket, buffer, 6, 0);
	// TODO handle error
}


bool NewConnectionEvent::handle_response(uint16_t opcode, RingBuffer &buffer) {
	switch(opcode) {
	case NEW_CONNECTION_ACK:
		return this->handle_new_connection(buffer);
	case NEW_CONNECTION_NACK:
		this->reject();
		return true;
	}
	return true;
}


bool NewConnectionEvent::supports_opcode(uint16_t opcode) const {
	return opcode == NEW_CONNECTION_ACK || opcode == NEW_CONNECTION_NACK;
}


bool NewConnectionEvent::handle_new_connection(RingBuffer &buffer) {
	if (!buffer.is_data_available(2)) {
		return false;
	}

	uint16_t connection_id = buffer.read_uint16();
	auto connection = this->get_connection();
	connection->initialize(connection_id, connection);
	this->resolve();
	return true;
}


}
