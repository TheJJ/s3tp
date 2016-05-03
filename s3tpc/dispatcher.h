#ifndef S3TPC_DISPATCHER_H_
#define S3TPC_DISPATCHER_H_


#include <cstdint>
#include <memory>
#include <thread>


namespace s3tpc {


class S3TPClient;


class Dispatcher {
public:
	static constexpr const uint64_t NOTIFICATION_VALUE = 1UL;

private:
	S3TPClient *parent;
	int notification_fd;
	int epoll_fd;

	bool is_running;
	std::unique_ptr<std::thread> main_worker;

public:
	Dispatcher(S3TPClient *parent);
	virtual ~Dispatcher();

	void start();
	void stop();

	void notify();

private:
	int init_notification_fd();
	int init_epoll_fd();
	void init_epoll_event(int epoll_fd, int fd, uint32_t events);

	void main_loop();
};


}


#endif
