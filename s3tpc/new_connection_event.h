#ifndef S3TPC_NEW_CONNECTION_EVENT_H_
#define S3TPC_NEW_CONNECTION_EVENT_H_


#include <cstdint>
#include <memory>

#include "connection.h"
#include "network_event.h"


namespace s3tpc {


class Dispatcher;


class NewConnectionEvent : public NetworkEvent {
private:
	std::shared_ptr<Connection> connection;

public:
	NewConnectionEvent(const std::shared_ptr<Connection> &connection, uint32_t id);
	virtual ~NewConnectionEvent() = default;

	void initialize_connection(uint16_t id);

	virtual void dispatch(Dispatcher *dispatcher);
	virtual bool handle_response(uint16_t opcode, RingBuffer &buffer);
};


}


#endif
