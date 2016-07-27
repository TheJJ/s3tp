#include "close_connection_event.h"

#include "opcodes.h"


namespace s3tpc {


CloseConnectionEvent::CloseConnectionEvent(const std::shared_ptr<Connection> &connection, uint32_t id)
	:
	RequestResponseEvent{connection, CLOSE_CONNECTION_BASE, id} {
}


bool CloseConnectionEvent::handle_acknowledgement(RingBuffer &buffer) {
	this->get_connection()->close();
	return true;
}


}
