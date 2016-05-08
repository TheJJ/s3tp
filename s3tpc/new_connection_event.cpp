#include "new_connection_event.h"

#include <sys/socket.h>
#include "dispatcher.h"


namespace s3tpc {


NewConnectionEvent::NewConnectionEvent(const std::shared_ptr<Connection> &connection)
	:
	connection{connection} {
}


void NewConnectionEvent::initialize_connection(uint16_t id) {
	this->connection->initialize(id);
	this->resolve();
}


void NewConnectionEvent::dispatch(Dispatcher *dispatcher) {
	int control_socket = dispatcher->get_control_socket();
}


}
