#ifndef S3TPC_RINGBUFFER_H_
#define S3TPC_RINGBUFFER_H_


#include <memory>
#include <stdexcept>
#include <string>


namespace s3tpc {


class RingBufferException : public std::runtime_error {
public:
	RingBufferException(const std::string &what);
};


class RingBuffer {
private:
	size_t length;
	size_t start;
	size_t end;
	std::unique_ptr<char[]> buffer;

	bool can_receive;
	size_t receive_buffer_size;
	std::unique_ptr<char[]> receive_buffer;

public:
	RingBuffer(size_t capacity, bool can_receive=false, size_t receive_buffer_size=4096);
	virtual ~RingBuffer() = default;

	size_t get_data(char *destination, size_t length, size_t offset=0) const;
	size_t read(char *destination, size_t length);
	size_t write(const char *source, size_t length);

	size_t recv(int sock);

	size_t discard(size_t length);

	bool is_empty() const;
	bool is_full() const;

	size_t get_capacity() const;
	size_t get_available_data(size_t offset=0) const;
	size_t get_available_space() const;

	bool is_data_available(size_t expected_size, size_t offset=0) const;

	uint16_t get_uint16(size_t offset=0) const;
	uint32_t get_uint32(size_t offset=0) const;

	uint16_t read_uint16();
	uint32_t read_uint32();

	void clear();
};


}


#endif
