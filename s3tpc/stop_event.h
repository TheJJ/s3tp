#ifndef S3TPC_STOP_EVENT_H_
#define S3TPC_STOP_EVENT_H_


#include <functional>

#include "event.h"


namespace s3tpc {


class StopEvent : public Event {
private:
	std::function<void()> stop_function;

public:
	StopEvent(std::function<void()> stop_function);
	virtual ~StopEvent() = default;

	virtual void dispatch(Dispatcher *dispatcher);
};


}


#endif
