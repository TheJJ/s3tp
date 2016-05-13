#include "event.h"


namespace s3tpc {


Event::Event()
	:
	Event{0} {
}


Event::Event(uint32_t id)
	:
	id{id} {
}


uint32_t Event::get_id() const {
	return this->id;
}


void Event::wait_for_resolution() {
	std::unique_lock<std::mutex> lock{this->mutex};
	this->resolver.wait(lock);
}


void Event::resolve() {
	this->resolver.notify_all();
}


}
