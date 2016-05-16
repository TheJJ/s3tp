#include "close_connection_event.h"

#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include "dispatcher.h"
#include "opcodes.h"


namespace s3tpc {


CloseConnectionEvent::CloseConnectionEvent(const std::shared_ptr<Connection> &connection, uint32_t id)
	:
	RequestResponseEvent{connection, CLOSE_CONNECTION_BASE, id} {
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


bool CloseConnectionEvent::handle_acknowledgement(RingBuffer &buffer) {
	this->get_connection()->close();
	return true;
}


}
