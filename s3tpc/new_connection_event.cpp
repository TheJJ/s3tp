#include "new_connection_event.h"

#include <cstring>
#include <sys/socket.h>
#include "opcodes.h"


namespace s3tpc {


NewConnectionEvent::NewConnectionEvent(const std::shared_ptr<Connection> &connection, uint32_t id)
	:
	RequestResponseEvent{connection, NEW_CONNECTION_BASE, id} {
}


void NewConnectionEvent::dispatch(Dispatcher *dispatcher) {
	int control_socket = dispatcher->get_control_socket();
	char buffer[6];

	this->init_header(buffer, NEW_CONNECTION_REQUEST);
	send(control_socket, buffer, sizeof(buffer), 0);
	// TODO handle error
}


bool NewConnectionEvent::handle_acknowledgement(RingBuffer &buffer) {
	if (!buffer.is_data_available(2)) {
		return false;
	}

	uint16_t connection_id = buffer.read_uint16();
	auto connection = this->get_connection();
	this->get_connection()->register_with_id(connection_id, connection);
	return true;
}


}
