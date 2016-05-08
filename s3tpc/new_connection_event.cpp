#include "new_connection_event.h"


namespace s3tpc {


NewConnectionEvent::NewConnectionEvent(const std::shared_ptr<Connection> &connection)
	:
	connection{connection} {
}


void NewConnectionEvent::initialize_connection(uint16_t id) {
	this->connection->initialize(id);
}


void NewConnectionEvent::dispatch(Dispatcher *dispatcher) {
	this->resolve();
}


}
