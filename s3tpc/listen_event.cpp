#include "listen_event.h"

#include <cstring>
#include "byteutils.h"
#include "opcodes.h"


namespace s3tpc {


ListenEvent::ListenEvent(const std::shared_ptr<Connection> &connection, uint16_t listen_port, uint32_t id)
	:
	RequestResponseEvent{connection, LISTEN_BASE, id},
	listen_port{listen_port} {
}


size_t ListenEvent::get_request_payload_size() const {
	return 2;
}


void ListenEvent::build_request_payload(char *buffer) const {
	write_uint16(buffer, this->listen_port);
}


bool ListenEvent::handle_acknowledgement(RingBuffer &buffer) {
	this->get_connection()->listen(this->listen_port);
	return true;
}


}
