#ifndef S3TPC_RECV_EVENT_H_
#define S3TPC_RECV_EVENT_H_


#include <cstdint>
#include <memory>

#include "connection.h"
#include "request_response_event.h"


namespace s3tpc {


class ReceiveEvent : public RequestResponseEvent {
private:
	uint16_t receive_length;
	char *target;
	uint16_t actual_received_size;

public:
	ReceiveEvent(const std::shared_ptr<Connection> &connection, char *target,
                 uint16_t receive_length, uint32_t id);
	virtual ~ReceiveEvent() = default;

	uint16_t get_actual_received_size() const;

protected:
	virtual size_t get_request_payload_size() const;
	virtual void build_request_payload(char *buffer) const;

	virtual bool handle_acknowledgement(RingBuffer &buffer);
};


}


#endif
