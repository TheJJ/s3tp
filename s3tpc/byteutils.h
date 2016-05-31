#ifndef S3TPC_BYTEUTILS_H_
#define S3TPC_BYTEUTILS_H_


#include <cstdint>


namespace s3tpc {


void write_uint16(char *buffer, uint16_t value);
void write_uint32(char *buffer, uint32_t value);


}


#endif
