#ifndef S3TPC_CLOSE_CONNECTION_EVENT_H_
#define S3TPC_CLOSE_CONNECTION_EVENT_H_


#include <cstdint>

#include "connection.h"
#include "network_event.h"


namespace s3tpc {


class CloseConnectionEvent : public NetworkEvent {
public:
	CloseConnectionEvent(const std::shared_ptr<Connection> &connection, uint32_t id);
	virtual ~CloseConnectionEvent() = default;

	virtual void dispatch(Dispatcher *dispatcher);
	virtual bool handle_response(uint16_t opcode, RingBuffer &buffer);

	virtual bool supports_opcode(uint16_t opcode) const;
};


}


#endif
