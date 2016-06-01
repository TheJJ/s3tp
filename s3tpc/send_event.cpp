#include "send_event.h"

#include <algorithm>
#include "byteutils.h"
#include "opcodes.h"


namespace s3tpc {


SendEvent::SendEvent(const std::shared_ptr<Connection> &connection, const char *data,
                     uint16_t send_length, uint32_t id)
	:
	RequestResponseEvent{connection, SEND_BASE, id},
	data{data, data + send_length} {
}


size_t SendEvent::get_request_payload_size() const {
	return 2 + this->data.size();
}


void SendEvent::build_request_payload(char *buffer) const {
	write_uint16(buffer, static_cast<uint16_t>(this->data.size()));
	std::copy(std::begin(this->data), std::end(this->data), buffer+2);
}


bool SendEvent::handle_acknowledgement(RingBuffer &buffer) {
	return true;
}


}
