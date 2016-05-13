#include "event.h"


namespace s3tpc {


Event::Event()
	:
	Event{0} {
}


Event::Event(uint32_t id)
	:
	id{id},
	succeeded{false} {
}


uint32_t Event::get_id() const {
	return this->id;
}


bool Event::has_succeeded() const {
	return this->succeeded;
}


void Event::wait_for_resolution() {
	std::unique_lock<std::mutex> lock{this->mutex};
	this->resolver.wait(lock);
}


void Event::resolve() {
	this->succeeded = true;
	this->resolver.notify_all();
}


void Event::reject() {
	this->succeeded = false;
	this->resolver.notify_all();
}


}
