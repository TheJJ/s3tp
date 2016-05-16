#ifndef S3TPC_CONNECT_EVENT_H_
#define S3TPC_CONNECT_EVENT_H_


#include <cstdint>

#include "connection.h"
#include "dispatcher.h"
#include "network_event.h"


namespace s3tpc {


class ConnectEvent : public NetworkEvent {
private:
	uint16_t destination_port;

public:
	ConnectEvent(const std::shared_ptr<Connection> &connection, uint16_t destination_port, uint32_t id);
	virtual ~ConnectEvent() = default;

	virtual void dispatch(Dispatcher *dispatcher);
	virtual bool handle_response(uint16_t opcode, RingBuffer &buffer);

	virtual bool supports_opcode(uint16_t opcode) const;

	bool handle_connected(RingBuffer &buffer);
};


}


#endif
