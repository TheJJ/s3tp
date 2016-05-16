#ifndef S3TPC_REQUEST_RESPONSE_EVENT_H_
#define S3TPC_REQUEST_RESPONSE_EVENT_H_


#include "network_event.h"


namespace s3tpc {


class RequestResponseEvent : public NetworkEvent {
private:
	uint16_t opcode_base;

public:
	RequestResponseEvent(const std::shared_ptr<Connection> &connection,
	                     uint16_t opcode_base, uint32_t id);
	virtual ~RequestResponseEvent() = default;

	virtual bool handle_response(uint16_t opcode, RingBuffer &buffer);
	virtual bool supports_opcode(uint16_t opcode) const;

protected:
	virtual bool handle_acknowledgement(RingBuffer &buffer) = 0;
};


}


#endif
