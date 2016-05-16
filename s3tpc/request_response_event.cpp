#include "request_response_event.h"


#include "opcodes.h"


namespace s3tpc {


RequestResponseEvent::RequestResponseEvent(const std::shared_ptr<Connection> &connection,
                                           uint16_t opcode_base, uint32_t id)
	:
	NetworkEvent{connection, id},
	opcode_base{opcode_base} {
}


bool RequestResponseEvent::handle_response(uint16_t opcode, RingBuffer &buffer) {
	if (opcode == this->opcode_base + ACK_OFFSET) {
		if (this->handle_acknowledgement(buffer)) {
			this->resolve();
			return true;
		}
		return false;
	} else if (opcode == this->opcode_base + NACK_OFFSET) {
		this->reject_and_close();
		return true;
	}
	// TODO assert illegal opcode
	return true;
}


bool RequestResponseEvent::supports_opcode(uint16_t opcode) const {
	return (opcode == this->opcode_base + ACK_OFFSET ||
	        opcode == this->opcode_base + NACK_OFFSET);
}


}
