#include "connect_event.h"

#include <cstring>
#include <sys/socket.h>
#include "opcodes.h"


namespace s3tpc {


ConnectEvent::ConnectEvent(const std::shared_ptr<Connection> &connection, uint16_t destination_port, uint32_t id)
	:
	RequestResponseEvent{connection, CONNECT_BASE, id},
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


bool ConnectEvent::handle_acknowledgement(RingBuffer &buffer) {
	if (!buffer.is_data_available(2)) {
		return false;
	}
	uint16_t local_port = buffer.read_uint16();
	this->get_connection()->connect_peer(local_port, this->destination_port);
	return true;
}


}
