#include "event.h"


namespace s3tpc {


void Event::wait_for_resolution() {
	std::unique_lock<std::mutex> lock{this->mutex};
	this->resolver.wait(lock);
}


void Event::resolve() {
	this->resolver.notify_all();
}


}
