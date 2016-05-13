#include "new_connection_event.h"

#include <sys/socket.h>
#include "dispatcher.h"
#include "opcodes.h"


namespace s3tpc {


NewConnectionEvent::NewConnectionEvent(const std::shared_ptr<Connection> &connection, uint32_t id)
	:
	Event{id},
	connection{connection} {
}


void NewConnectionEvent::initialize_connection(uint16_t id) {
	this->connection->initialize(id);
	this->resolve();
}


void NewConnectionEvent::dispatch(Dispatcher *dispatcher) {
	int control_socket = dispatcher->get_control_socket();

	uint16_t opcode = NEW_CONNECTION_REQUEST;
	uint32_t event_id = this->get_id();
	send(control_socket, &opcode, 2, 0);
	send(control_socket, &event_id, 4, 0);
}


}
