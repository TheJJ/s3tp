#include "network_event.h"

#include <cstring>


namespace s3tpc {


NetworkEvent::NetworkEvent(const std::shared_ptr<Connection> &connection, uint32_t id)
	:
	Event{id},
	connection{connection} {
}


std::shared_ptr<Connection> NetworkEvent::get_connection() {
	return this->connection;
}


void NetworkEvent::reject_and_close(uint16_t error) {
	this->connection->close_due_to_error();
	this->reject(error);
}


}
