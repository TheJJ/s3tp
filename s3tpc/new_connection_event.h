#ifndef S3TPC_NEW_CONNECTION_EVENT_H_
#define S3TPC_NEW_CONNECTION_EVENT_H_


#include <memory>

#include "connection.h"
#include "event.h"


namespace s3tpc {


class Dispatcher;


class NewConnectionEvent : public Event {
private:
	std::shared_ptr<Connection> connection;

public:
	NewConnectionEvent(const std::shared_ptr<Connection> &connection);
	virtual ~NewConnectionEvent() = default;

	void initialize_connection(uint16_t id);

	virtual void dispatch(Dispatcher *dispatcher);
};


}


#endif
