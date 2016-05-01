#ifndef S3TPC_RINGBUFFER_H_
#define S3TPC_RINGBUFFER_H_


#include <unistd.h>


struct ring_buffer;

struct ring_buffer *rb_create(size_t capacity);
void rb_destroy(struct ring_buffer *rb);

ssize_t rb_read(struct ring_buffer *rb, char *destination, size_t length);
ssize_t rb_write(struct ring_buffer *rb, const char *source, size_t length);

int rb_is_empty(struct ring_buffer *rb);
int rb_is_full(struct ring_buffer *rb);
size_t rb_capacity(struct ring_buffer *rb);
size_t rb_available_data(struct ring_buffer *rb);
size_t rb_available_space(struct ring_buffer *rb);


#endif
