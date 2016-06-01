#ifndef S3TPC_SEND_EVENT_H_
#define S3TPC_SEND_EVENT_H_


#include <cstdint>
#include <memory>
#include <vector>

#include "connection.h"
#include "request_response_event.h"


namespace s3tpc {


class SendEvent : public RequestResponseEvent {
private:
	std::vector<char> data;

public:
	SendEvent(const std::shared_ptr<Connection> &connection, const char *data,
	          uint16_t send_length, uint32_t id);
	virtual ~SendEvent() = default;

protected:
	virtual size_t get_request_payload_size() const override;
	virtual void build_request_payload(char *buffer) const override;

	virtual bool handle_acknowledgement(RingBuffer &buffer) override;
};


}


#endif
