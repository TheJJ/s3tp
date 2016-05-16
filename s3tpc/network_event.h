#ifndef S3TPC_NETWORK_EVENT_H_
#define S3TPC_NETWORK_EVENT_H_


#include <cstdint>
#include <memory>
#include "event.h"
#include "connection.h"
#include "ringbuffer.h"


namespace s3tpc {


class NetworkEvent : public Event {
private:
	std::shared_ptr<Connection> connection;

public:
	NetworkEvent(const std::shared_ptr<Connection> &connection, uint32_t id);
	virtual ~NetworkEvent() = default;

	virtual bool handle_response(uint16_t opcode, RingBuffer &buffer) = 0;
	virtual bool supports_opcode(uint16_t opcode) const = 0;

protected:
	std::shared_ptr<Connection> get_connection();

	void init_header(char *buffer, uint16_t opcode);

	void reject_and_close();
};


}


#endif
