#include "ringbuffer.h"


#include <algorithm>
#include <cstring>


namespace s3tpc {


RingBuffer::RingBuffer(size_t capacity)
		:
		length{capacity + 1},
		start{0},
		end{0},
		buffer{std::make_unique<char[]>(length)} {
}


size_t RingBuffer::read(char *destination, size_t length) {
	length = std::min(length, this->get_available_data());
	if (this->start <= this->end) {
		memcpy(destination, &this->buffer[this->start], length);
		this->start += length;
	} else {
		size_t first_size = std::min(this->length - this->start, length);
		memcpy(destination, &this->buffer[this->start], first_size);
		size_t second_size = length - first_size;
		if (second_size > 0) {
			memcpy(destination + first_size, this->buffer.get(), second_size);
			this->start = second_size;
		} else {
			this->start = (this->start + first_size) % this->length;
		}
	}
	return length;
}


size_t RingBuffer::write(const char *source, size_t length) {
	if (length > this->get_available_space()) {
		// TODO throw error or return zero
	}

	if (this->end <= this->start) {
		memcpy(&this->buffer[this->end], source, length);
		this->end = (this->end + length) % this->length;
	} else {
		size_t first_size = std::min(this->length - this->end, length);
		memcpy(&this->buffer[this->end], source, first_size);
		size_t second_size = length - first_size;
		if (second_size > 0) {
			memcpy(this->buffer.get(), source + first_size, second_size);
			this->end = second_size;
		} else {
			this->end = (this->end + first_size) % this->length;
		}
	}
	return length;
}


bool RingBuffer::is_empty() const {
	return this->get_available_data() == 0;
}


bool RingBuffer::is_full() const {
	return this->get_available_space() == 0;
}


size_t RingBuffer::get_capacity() const {
	return this->length - 1;
}


size_t RingBuffer::get_available_data() const {
	if (this->start <= this->end) {
		return this->end - this->start;
	}
	return this->length - (this->start - this->end);
}


size_t RingBuffer::get_available_space() const {
	return this->get_capacity() - this->get_available_data();
}


}
