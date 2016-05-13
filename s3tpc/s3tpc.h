#ifndef S3TPC_S3TPC_H_
#define S3TPC_S3TPC_H_


#include <atomic>
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>

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

	void connect(const std::string &socket_path);
	void start();
	void stop();

	int get_control_socket() const;

	std::shared_ptr<Connection> create_connection();
	bool close_connection(uint16_t id);

	void dispatch_incoming_data();

	void register_connection(const std::shared_ptr<Connection> &connection);
	void deregister_connection(uint16_t id);

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
