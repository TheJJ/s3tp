#ifndef S3TPC_CONNECTION_H_
#define S3TPC_CONNECTION_H_


#include <cstdint>
#include <memory>


namespace s3tpc {


class S3TPClient;


enum class ConnectionState {
	NEW,
	REGISTERED,
	LISTENING,
	CONNECTED_LISTENER,
	CONNECTED_PEER,
	CLOSED
};


class Connection {
public:
	static constexpr const uint16_t NEW_CONNECTION_ID = 0;

private:
	S3TPClient *parent;
	uint16_t id;
	uint16_t local_port;
	uint16_t remote_port;
	ConnectionState state;

public:
	Connection(S3TPClient *parent);
	virtual ~Connection() = default;

	void register_with_id(uint16_t id, const std::shared_ptr<Connection> &tracked_self);
	void connect_peer(uint16_t local_port, uint16_t remote_port);
	void listen(uint16_t listen_port);
	void connect_listener(uint16_t remote_port);
	void close_due_to_error();
	void close();

	uint16_t get_id() const;
	uint16_t get_local_port() const;
	uint16_t get_remote_port() const;

	bool is_connected() const;

	bool has_state(const ConnectionState &state) const;
	ConnectionState get_state() const;
};


}


#endif
