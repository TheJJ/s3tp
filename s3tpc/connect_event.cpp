#include "connect_event.h"

#include <cstring>
#include <sys/socket.h>
#include "opcodes.h"


namespace s3tpc {


ConnectEvent::ConnectEvent(const std::shared_ptr<Connection> &connection, uint16_t destination_port, uint32_t id)
	:
	NetworkEvent{connection, id},
	destination_port{destination_port} {
}


void ConnectEvent::dispatch(Dispatcher *dispatcher) {
	int control_socket = dispatcher->get_control_socket();
	char buffer[10];
	uint16_t connection_id = this->get_connection()->get_id();

	this->init_header(buffer, CONNECT_REQUEST);
	memcpy(buffer+6, &connection_id, 2);
	memcpy(buffer+8, &this->destination_port, 2);
	send(control_socket, buffer, sizeof(buffer), 0);
}


bool ConnectEvent::handle_response(uint16_t opcode, RingBuffer &buffer) {
	switch(opcode) {
	case CONNECT_ACK:
		this->handle_connected(buffer);
		return true;
	case CONNECT_NACK:
		this->reject();
		return true;
	}
	return true;
}


bool ConnectEvent::supports_opcode(uint16_t opcode) const {
	return opcode == CONNECT_ACK || opcode == CONNECT_NACK;
}


bool ConnectEvent::handle_connected(RingBuffer &buffer) {
	if (!buffer.is_data_available(2)) {
		return false;
	}
	uint16_t local_port = buffer.read_uint16();
	this->get_connection()->initialize_ports(local_port, this->destination_port);
	this->resolve();
	return true;
}


}
