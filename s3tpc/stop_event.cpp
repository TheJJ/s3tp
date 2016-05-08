#include "stop_event.h"


namespace s3tpc {


StopEvent::StopEvent(std::function<void()> stop_function)
	:
	stop_function{stop_function} {
}


void StopEvent::dispatch(Dispatcher *dispatcher) {
	this->stop_function();
}


}
