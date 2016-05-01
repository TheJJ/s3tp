#include "ringbuffer.h"

#include <stdlib.h>
#include <string.h>

#include "util.h"


struct ring_buffer {
	char *buffer;
	size_t length;
	size_t start;
	size_t end;
};


struct ring_buffer *rb_create(size_t capacity) {
	struct ring_buffer *rb = malloc(sizeof(struct ring_buffer));
	if (rb == NULL) {
		return NULL;
	}
	rb->length = capacity + 1;
	rb->start = 0;
	rb->end = 0;
	rb->buffer = malloc(rb->length);
	return rb;
}


void rb_destroy(struct ring_buffer *rb) {
	if (rb == NULL) {
		return;
	}
	if (rb->buffer != NULL) {
		free(rb->buffer);
	}
	free(rb);
}


ssize_t rb_read(struct ring_buffer *rb, char *destination, size_t length) {
	size_t first_size;
	size_t second_size;

	length = MIN(length, rb_available_data(rb));
	if (rb->start <= rb->end) {
		memcpy(destination, rb->buffer + rb->start, length);
		rb->start += length;
	} else {
		first_size = MIN(rb->length - rb->start, length);
		memcpy(destination, rb->buffer + rb->start, first_size);
		second_size = length - first_size;
		if (second_size > 0) {
			memcpy(destination + first_size, rb->buffer, second_size);
			rb->start = second_size;
		} else {
			rb->start = (rb->start + first_size) % rb->length;
		}
	}
	return length;
}


ssize_t rb_write(struct ring_buffer *rb, const char *source, size_t length) {
	size_t first_size;
	size_t second_size;

	if (length > rb_available_space(rb)) {
		return -1;
	}

	if (rb->end <= rb->start) {
		memcpy(rb->buffer + rb->end, source, length);
		rb->end = (rb->end + length) % rb->length;
	} else {
		first_size = MIN(rb->length - rb->end, length);
		memcpy(rb->buffer + rb->end, source, first_size);
		second_size = length - first_size;
		if (second_size > 0) {
			memcpy(rb->buffer, source + first_size, second_size);
			rb->end = second_size;
		} else {
			rb->end = (rb->end + first_size) % rb->length;
		}
	}
	return length;
}


int rb_is_empty(struct ring_buffer *rb) {
	return rb_available_data(rb) == 0;
}


int rb_is_full(struct ring_buffer *rb) {
	return rb_available_space(rb) == 0;
}


size_t rb_capacity(struct ring_buffer *rb) {
	return rb->length - 1;
}


size_t rb_available_data(struct ring_buffer *rb) {
	if (rb->start <= rb->end) {
		return rb->end - rb->start;
	}
	return rb->length - (rb->start - rb->end);
}


size_t rb_available_space(struct ring_buffer *rb) {
	return rb_capacity(rb) - rb_available_data(rb);
}
