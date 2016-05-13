#ifndef S3TPC_CONTROLLER_H_
#define S3TPC_CONTROLLER_H_


#include <atomic>
#include <cstddef>
#include <memory>
#include <mutex>
#include <unordered_map>

#include "ringbuffer.h"


namespace s3tpc {


class Connection;
class NewConnectionEvent;
class S3TPClient;


class ProtocolHandler {
public:
	static constexpr const size_t RECEIVE_SIZE = 4096;
	static constexpr const size_t BUFFER_SIZE = 65536;

private:
	S3TPClient *parent;
	int control_socket;

	RingBuffer buffer;

	std::atomic<uint32_t> event_id_counter;

	std::mutex new_connection_mutex;
	std::unordered_map<uint32_t,std::shared_ptr<NewConnectionEvent>> new_connection_requests;

public:
	ProtocolHandler(S3TPClient *parent);
	virtual ~ProtocolHandler() = default;

	std::shared_ptr<NewConnectionEvent> register_new_connection(const std::shared_ptr<Connection> &connection);

	void dispatch_incoming_data();

private:
	int read_message_opcode();

	void handle_new_connection(size_t response_size, bool success);
};


}


#endif
