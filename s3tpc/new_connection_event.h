#ifndef S3TPC_NEW_CONNECTION_EVENT_H_
#define S3TPC_NEW_CONNECTION_EVENT_H_


#include <cstdint>
#include <memory>

#include "connection.h"
#include "dispatcher.h"
#include "network_event.h"


namespace s3tpc {


class NewConnectionEvent : public NetworkEvent {
public:
	NewConnectionEvent(const std::shared_ptr<Connection> &connection, uint32_t id);
	virtual ~NewConnectionEvent() = default;

	virtual void dispatch(Dispatcher *dispatcher);
	virtual bool handle_response(uint16_t opcode, RingBuffer &buffer);

	virtual bool supports_opcode(uint16_t opcode) const;

	bool handle_new_connection(RingBuffer &buffer);
};


}


#endif
