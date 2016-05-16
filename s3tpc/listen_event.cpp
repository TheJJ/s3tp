#include "listen_event.h"

#include <cstring>
#include <sys/socket.h>
#include "opcodes.h"


namespace s3tpc {


ListenEvent::ListenEvent(const std::shared_ptr<Connection> &connection, uint16_t listen_port, uint32_t id)
	:
	RequestResponseEvent{connection, LISTEN_BASE, id},
	listen_port{listen_port} {
}


void ListenEvent::dispatch(Dispatcher *dispatcher) {
	int control_socket = dispatcher->get_control_socket();
	char buffer[10];
	uint16_t connection_id = this->get_connection()->get_id();

	this->init_header(buffer, LISTEN_REQUEST);
	memcpy(buffer+6, &connection_id, 2);
	memcpy(buffer+8, &this->listen_port, 2);
	send(control_socket, buffer, sizeof(buffer), 0);
}


bool ListenEvent::handle_acknowledgement(RingBuffer &buffer) {
	this->get_connection()->listen(this->listen_port);
	return true;
}


}
