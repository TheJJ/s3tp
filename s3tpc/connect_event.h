#ifndef S3TPC_CONNECT_EVENT_H_
#define S3TPC_CONNECT_EVENT_H_


#include <cstdint>

#include "connection.h"
#include "dispatcher.h"
#include "request_response_event.h"


namespace s3tpc {


class ConnectEvent : public RequestResponseEvent {
private:
	uint16_t destination_port;

public:
	ConnectEvent(const std::shared_ptr<Connection> &connection, uint16_t destination_port, uint32_t id);
	virtual ~ConnectEvent() = default;

protected:
	virtual size_t get_request_payload_size() const;
	virtual void build_request_payload(char *buffer) const;

	virtual bool handle_acknowledgement(RingBuffer &buffer);
};


}


#endif
