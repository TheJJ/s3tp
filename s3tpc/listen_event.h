#ifndef S3TPC_LISTEN_EVENT_H_
#define S3TPC_LISTEN_EVENT_H_


#include <cstdint>
#include <memory>

#include "connection.h"
#include "request_response_event.h"


namespace s3tpc {


class ListenEvent : public RequestResponseEvent {
private:
	uint16_t listen_port;

public:
	ListenEvent(const std::shared_ptr<Connection> &connection, uint16_t listen_port, uint32_t id);
	virtual ~ListenEvent() = default;

protected:
	virtual size_t get_request_payload_size() const override;
	virtual void build_request_payload(char *buffer) const override;

	virtual bool handle_acknowledgement(RingBuffer &buffer) override;
};


}


#endif
