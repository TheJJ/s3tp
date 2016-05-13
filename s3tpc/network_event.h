#ifndef S3TPC_NETWORK_EVENT_H_
#define S3TPC_NETWORK_EVENT_H_


#include <cstdint>
#include "event.h"
#include "ringbuffer.h"


namespace s3tpc {


class NetworkEvent : public Event {
public:
	NetworkEvent(uint32_t id);
	virtual ~NetworkEvent() = default;

	virtual bool handle_response(uint16_t opcode, RingBuffer &buffer) = 0;
};


}


#endif
