#include "wait_for_peer_event.h"

#include "opcodes.h"


namespace s3tpc {


WaitForPeerEvent::WaitForPeerEvent(const std::shared_ptr<Connection> &connection, uint32_t id)
	:
	RequestResponseEvent{connection, WAIT_FOR_PEER_BASE, id} {
}


bool WaitForPeerEvent::handle_acknowledgement(RingBuffer &buffer) {
	if (!buffer.is_data_available(2)) {
		return false;
	}
	uint16_t remote_port = buffer.read_uint16();
	this->get_connection()->connect_listener(remote_port);
	return true;
}


}
