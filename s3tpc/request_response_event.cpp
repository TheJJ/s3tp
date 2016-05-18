#include "request_response_event.h"


#include <cstring>
#include <sys/socket.h>
#include "dispatcher.h"
#include "opcodes.h"


namespace s3tpc {


RequestResponseEvent::RequestResponseEvent(const std::shared_ptr<Connection> &connection,
                                           uint16_t opcode_base, uint32_t id)
	:
	NetworkEvent{connection, id},
	opcode_base{opcode_base} {
}


void RequestResponseEvent::dispatch(Dispatcher *dispatcher) {
	uint16_t opcode = this->opcode_base + REQUEST_OFFSET;
	uint32_t event_id = this->get_id();
	bool include_connection_id = this->request_has_connection_id();
	size_t request_size = this->get_request_payload_size() + 6;
	size_t payload_offset = 6;

	if (include_connection_id) {
		request_size += 2;
		payload_offset += 2;
	}

	char buffer[request_size];
	memcpy(buffer, &opcode, 2);
	memcpy(buffer+2, &event_id, 4);
	if (include_connection_id) {
		uint16_t connection_id = this->get_connection()->get_id();
		memcpy(buffer+6, &connection_id, 2);
	}

	this->build_request_payload(buffer + payload_offset);

	send(dispatcher->get_control_socket(), buffer, request_size, 0);
	// TODO handle error
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


bool RequestResponseEvent::request_has_connection_id() const {
	return true;
}


size_t RequestResponseEvent::get_request_payload_size() const {
	return 0;
}


void RequestResponseEvent::build_request_payload(char *buffer) const {
	// do nothing by default
}


}
