#include "receive_event.h"

#include "byteutils.h"
#include "opcodes.h"


namespace s3tpc {


ReceiveEvent::ReceiveEvent(const std::shared_ptr<Connection> &connection, char *target,
                           uint16_t receive_length, uint32_t id)
	:
	RequestResponseEvent{connection, RECV_BASE, id},
	receive_length{receive_length},
	target{target},
	actual_received_size{0} {
}


uint16_t ReceiveEvent::get_actual_received_size() const {
	return this->actual_received_size;
}


size_t ReceiveEvent::get_request_payload_size() const {
	return 2;
}


void ReceiveEvent::build_request_payload(char *buffer) const {
	write_uint16(buffer, this->receive_length);
}


bool ReceiveEvent::handle_acknowledgement(RingBuffer &buffer) {
	if (!buffer.is_length_prefixed_data_available()) {
		return false;
	}
	this->actual_received_size = buffer.read_uint16();
	buffer.read(this->target, this->actual_received_size);
	return true;
}


}
