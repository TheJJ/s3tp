#include "ringbuffer.h"


#include <algorithm>
#include <cstring>
#include <iostream>
#include <system_error>
#include <sys/socket.h>


namespace s3tpc {


RingBufferException::RingBufferException(const std::string &what)
	:
	std::runtime_error{what} {
}


RingBuffer::RingBuffer(size_t capacity, bool can_receive, size_t receive_buffer_size)
		:
		length{capacity + 1},
		start{0},
		end{0},
		buffer{std::make_unique<char[]>(length)},
		can_receive{can_receive},
		receive_buffer_size{receive_buffer_size} {
	if (this->can_receive) {
		auto buffer = std::make_unique<char[]>(receive_buffer_size);
		this->receive_buffer.swap(buffer);
	}
}


size_t RingBuffer::get_data(char *destination, size_t length, size_t offset) const {
	size_t available = this->get_available_data();
	if (available <= offset) {
		return 0;
	}
	length = std::min(length, available - offset);
	if (this->start + offset <= this->end) {
		memcpy(destination, &this->buffer[this->start + offset], length);
	} else {
		size_t first_size = std::min(this->length - this->start + offset, length);
		memcpy(destination, &this->buffer[this->start + offset], first_size);
		size_t second_size = length - first_size;
		if (second_size > 0) {
			memcpy(destination + first_size, this->buffer.get(), second_size);
		}
	}
	return length;
}


size_t RingBuffer::read(char *destination, size_t length) {
	size_t read_length = this->get_data(destination, length);
	this->start = (this->start + read_length) % this->length;
	return read_length;
}


size_t RingBuffer::write(const char *source, size_t length) {
	if (length > this->get_available_space()) {
		throw RingBufferException{"There is not enough space for writing."};
	}

	if (this->end <= this->start) {
		memcpy(&this->buffer[this->end], source, length);
	} else {
		size_t first_size = std::min(this->length - this->end, length);
		memcpy(&this->buffer[this->end], source, first_size);
		size_t second_size = length - first_size;
		if (second_size > 0) {
			memcpy(this->buffer.get(), source + first_size, second_size);
		}
	}
	this->end = (this->end + length) % this->length;
	return length;
}


size_t RingBuffer::recv(int sock) {
	if (!this->can_receive) {
		throw RingBufferException{"RingBuffer cannot receive."};
	}
	if (this->receive_buffer_size > this->get_available_space()) {
		throw RingBufferException{"There is not enough space for receiving."};
	}

	ssize_t received_size = ::recv(sock, this->receive_buffer.get(), this->receive_buffer_size, 0);
	if (received_size < 0) {
		throw std::system_error{errno, std::system_category()};
	}

	this->write(this->receive_buffer.get(), received_size);
	return received_size;
}


size_t RingBuffer::discard(size_t length) {
	length = std::min(length, this->get_available_data());
	this->start = (this->start + length) % this->length;
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


size_t RingBuffer::get_available_data(size_t offset) const {
	size_t available_data = 0;
	if (this->start <= this->end) {
		available_data = this->end - this->start;
	} else {
		available_data = this->length - (this->start - this->end);
	}
	if (offset > available_data) {
		available_data = 0;
	} else {
		available_data -= offset;
	}
	return available_data;
}


size_t RingBuffer::get_available_space() const {
	return this->get_capacity() - this->get_available_data();
}


bool RingBuffer::is_data_available(size_t expected_size, size_t offset) const {
	return this->get_available_data(offset) >= expected_size;
}


uint16_t RingBuffer::get_uint16(size_t offset) const {
	uint16_t value;
	if (this->get_data(reinterpret_cast<char*>(&value), sizeof(value), offset) != sizeof(value)) {
		throw RingBufferException{"There is not enough data available."};
	}
	return value;
}


uint32_t RingBuffer::get_uint32(size_t offset) const {
	uint32_t value;
	if (this->get_data(reinterpret_cast<char*>(&value), sizeof(value), offset) != sizeof(value)) {
		throw RingBufferException{"There is not enough data available."};
	}
	return value;
}


uint16_t RingBuffer::read_uint16() {
	uint16_t value = this->get_uint16();
	this->discard(sizeof(uint16_t));
	return value;
}


uint32_t RingBuffer::read_uint32() {
	uint32_t value = this->get_uint32();
	this->discard(sizeof(uint32_t));
	return value;
}


void RingBuffer::clear() {
	this->start = 0;
	this->end = 0;
}


}
