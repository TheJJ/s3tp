#include "event.h"


namespace s3tpc {


Event::Event()
	:
	Event{0} {
}


Event::Event(uint32_t id)
	:
	id{id},
	succeeded{false},
	error{0} {
}


uint32_t Event::get_id() const {
	return this->id;
}


uint16_t Event::get_error() const {
	return this->error;
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


void Event::reject(uint16_t error) {
	this->succeeded = false;
	this->error = error;
	this->resolver.notify_all();
}


}
