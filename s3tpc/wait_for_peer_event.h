#ifndef S3TPC_WAIT_FOR_PEER_EVENT_H_
#define S3TPC_WAIT_FOR_PEER_EVENT_H_


#include <cstdint>
#include <memory>

#include "connection.h"
#include "request_response_event.h"


namespace s3tpc {


class WaitForPeerEvent : public RequestResponseEvent {
public:
	WaitForPeerEvent(const std::shared_ptr<Connection> &connection, uint32_t id);
	virtual ~WaitForPeerEvent() = default;

protected:
	virtual bool handle_acknowledgement(RingBuffer &buffer);
};


}


#endif
