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
class NetworkEvent;
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

	std::mutex events_mutex;
	std::unordered_map<uint32_t,std::shared_ptr<NetworkEvent>> events;

	int current_opcode;
	long current_event_id;

public:
	ProtocolHandler(S3TPClient *parent);
	virtual ~ProtocolHandler() = default;

	std::shared_ptr<NetworkEvent> register_new_connection(const std::shared_ptr<Connection> &connection);

	void dispatch_incoming_data();

	bool read_opcode_and_event_id();
	void reset_opcode_and_event_id();
};


}


#endif
