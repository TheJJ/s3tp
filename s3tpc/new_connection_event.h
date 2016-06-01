#ifndef S3TPC_NEW_CONNECTION_EVENT_H_
#define S3TPC_NEW_CONNECTION_EVENT_H_


#include <cstdint>
#include <memory>

#include "connection.h"
#include "request_response_event.h"


namespace s3tpc {


class NewConnectionEvent : public RequestResponseEvent {
public:
	NewConnectionEvent(const std::shared_ptr<Connection> &connection, uint32_t id);
	virtual ~NewConnectionEvent() = default;

protected:
	virtual bool request_has_connection_id() const override;

	virtual bool handle_acknowledgement(RingBuffer &buffer) override;
};


}


#endif
