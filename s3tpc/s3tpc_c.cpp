#include "s3tpc_c.h"

#include <system_error>
#include "s3tpc.h"


extern "C" {


int s3tp_init(const char *socket_path) {
	try {
		s3tpc::S3TPClient &client = s3tpc::S3TPClient::get_instance();
		client.connect(socket_path);
		client.start();
	} catch(const std::system_error &e) {
		// negative value of the internal errno
		return -e.code().value();
	}
	return 0;
}


void s3tp_destroy() {
	try {
		s3tpc::S3TPClient &client = s3tpc::S3TPClient::get_instance();
		client.stop();
	} catch(const std::system_error &e) {
		// ignore error
	}
}


int s3tp_create() {
	s3tpc::S3TPClient &client = s3tpc::S3TPClient::get_instance();
	// TODO handle error
	auto connection = client.create_connection();
	if (connection->is_initialized()) {
		return connection->get_id();
	}
	return -1;
}


int s3tp_send(int connection, const char *data, size_t length) {
	return 0;
}


int s3tp_receive(int connection, char *data, size_t length) {
	return 0;
}


}
