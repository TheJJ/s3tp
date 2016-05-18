#include "new_connection_event.h"

#include <cstring>
#include <sys/socket.h>
#include "opcodes.h"


namespace s3tpc {


NewConnectionEvent::NewConnectionEvent(const std::shared_ptr<Connection> &connection, uint32_t id)
	:
	RequestResponseEvent{connection, NEW_CONNECTION_BASE, id} {
}


bool NewConnectionEvent::request_has_connection_id() const {
	return false;
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
