#include "network_event.h"


namespace s3tpc {


NetworkEvent::NetworkEvent(const std::shared_ptr<Connection> &connection, uint32_t id)
	:
	Event{id},
	connection{connection} {
}


std::shared_ptr<Connection> NetworkEvent::get_connection() {
	return this->connection;
}


}
