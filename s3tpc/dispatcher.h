#ifndef S3TPC_DISPATCHER_H_
#define S3TPC_DISPATCHER_H_


#include <cstdint>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <sys/epoll.h>

#include "event.h"


namespace s3tpc {


class S3TPClient;


class Dispatcher {
public:
	static constexpr const size_t MAX_EVENTS = 64;
	static constexpr const uint64_t NOTIFICATION_VALUE = 1UL;

private:
	S3TPClient *parent;
	int notification_fd;
	int epoll_fd;

	bool is_running;
	std::unique_ptr<std::thread> main_worker;

	std::mutex event_mutex;
	std::queue<std::shared_ptr<Event>> pending_events;

public:
	Dispatcher(S3TPClient *parent);
	virtual ~Dispatcher();

	void start();
	void stop();

	void push_event(const std::shared_ptr<Event> &event);

	int get_control_socket() const;

private:
	void notify();

	int init_notification_fd();
	int init_epoll_fd();
	void init_epoll_event(int epoll_fd, int fd, uint32_t events);

	void main_loop();

	uint64_t get_notification_count(struct epoll_event *event);

	void dispatch_queue(uint64_t event_count);
	void dispatch_event(struct epoll_event *event);
};


}


#endif
