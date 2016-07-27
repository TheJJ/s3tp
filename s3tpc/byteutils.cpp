#include "byteutils.h"

#include <cstring>
#include <netinet/in.h>


namespace s3tpc {


void write_uint16(char *buffer, uint16_t value) {
	uint16_t ordered_value = htons(value);
	memcpy(buffer, &ordered_value, sizeof(uint16_t));
}


void write_uint32(char *buffer, uint32_t value) {
	uint32_t ordered_value = htonl(value);
	memcpy(buffer, &ordered_value, sizeof(uint32_t));
}


}
