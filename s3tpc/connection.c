#include "connection.h"
#include "connection_internal.h"

#include <stdint.h>

#include "hashmap.h"
#include "ringbuffer.h"


#define DEFAULT_CONNECTION_MAP_SIZE 128


struct s3tp_connection {
	uint16_t id;
	struct ring_buffer *send_buffer;
	struct ring_buffer *recv_buffer;
};


static struct hash_map *connection_map;


int init_connections() {
	connection_map = hm_create(DEFAULT_CONNECTION_MAP_SIZE);
	return 0;
}


void destroy_connections() {
	// TODO tear down connections
	hm_destroy(connection_map);
}


struct s3tp_connection *s3tp_create() {
}
