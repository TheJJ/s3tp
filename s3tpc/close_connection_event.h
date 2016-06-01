#ifndef S3TPC_CLOSE_CONNECTION_EVENT_H_
#define S3TPC_CLOSE_CONNECTION_EVENT_H_


#include <cstdint>
#include <memory>

#include "connection.h"
#include "request_response_event.h"


namespace s3tpc {


class CloseConnectionEvent : public RequestResponseEvent {
public:
	CloseConnectionEvent(const std::shared_ptr<Connection> &connection, uint32_t id);
	virtual ~CloseConnectionEvent() = default;

protected:
	virtual bool handle_acknowledgement(RingBuffer &buffer) override;
};


}


#endif
