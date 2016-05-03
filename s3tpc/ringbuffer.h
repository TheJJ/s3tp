#ifndef S3TPC_RINGBUFFER_H_
#define S3TPC_RINGBUFFER_H_


#include <memory>


namespace s3tpc {


class RingBuffer {
private:
	size_t length;
	size_t start;
	size_t end;
	std::unique_ptr<char[]> buffer;

public:
	RingBuffer(size_t capacity);
	virtual ~RingBuffer() = default;

	size_t read(char *destination, size_t length);
	size_t write(const char *source, size_t length);

	bool is_empty() const;
	bool is_full() const;

	size_t get_capacity() const;
	size_t get_available_data() const;
	size_t get_available_space() const;
};


}


#endif
