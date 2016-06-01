#ifndef S3TPC_S3TPC_H_
#define S3TPC_S3TPC_H_


#include <atomic>
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "connection.h"
#include "dispatcher.h"
#include "protocol_handler.h"


namespace s3tpc {


class S3TPClient {
private:
	int control_socket;
	ProtocolHandler protocol_handler;
	Dispatcher dispatcher;

	std::mutex connections_mutex;
	std::unordered_map<uint16_t,std::shared_ptr<Connection>> connections;

public:
	S3TPClient();
	virtual ~S3TPClient();

	void connect_to_s3tpd(const std::string &socket_path);
	void start();
	void stop();

	int get_control_socket() const;

	std::shared_ptr<Connection> create_connection();
	bool close_connection(uint16_t id);

	bool connect(uint16_t id, uint16_t port);
	bool listen(uint16_t id, uint16_t port);
	bool wait_for_peer(uint16_t id);

	int receive(uint16_t id, char *destination, uint16_t length);
	bool send(uint16_t id, const char *data, uint16_t length);

	void dispatch_incoming_data();

	void register_connection(const std::shared_ptr<Connection> &connection);
	void deregister_connection(uint16_t id);

	std::shared_ptr<Connection> get_connection(uint16_t id);

protected:
	template<typename EventType, typename... Args>
	std::shared_ptr<EventType> create_and_wait_for_connection_event(uint16_t id,
			const std::unordered_set<ConnectionState> &states, Args&& ...args) {
		auto connection = this->get_connection(id);
		if (!connection || states.find(connection->get_state()) == std::end(states))  {
			return nullptr;
		}
		auto event = this->protocol_handler.register_event<EventType>(connection, std::forward<Args>(args)...);
		this->dispatcher.push_event(event);
		event->wait_for_resolution();
		return event;
	}

	template<typename EventType, typename... Args>
	bool create_and_wait_for_connection_event_succeeded(uint16_t id,
			const std::unordered_set<ConnectionState> &states, Args&& ...args) {
		auto event = this->create_and_wait_for_connection_event<EventType>(id, states, std::forward<Args>(args)...);
		if (event) {
			return event->has_succeeded();
		}
		return false;
	}

private:
	int init_socket();
	void unblock_socket(int sock);

private:
	static std::unique_ptr<S3TPClient> instance;

public:
	static S3TPClient &get_instance();
};


}


#endif
