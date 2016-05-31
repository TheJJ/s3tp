#include "connect_event.h"

#include <cstring>
#include "opcodes.h"


namespace s3tpc {


ConnectEvent::ConnectEvent(const std::shared_ptr<Connection> &connection, uint16_t destination_port, uint32_t id)
	:
	RequestResponseEvent{connection, CONNECT_BASE, id},
	destination_port{destination_port} {
}


size_t ConnectEvent::get_request_payload_size() const {
	return 2;
}


void ConnectEvent::build_request_payload(char *buffer) const {
	memcpy(buffer, &this->destination_port, 2);
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
