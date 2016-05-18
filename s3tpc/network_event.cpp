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


void NetworkEvent::reject_and_close() {
	this->connection->close();
	this->reject();
}


}
