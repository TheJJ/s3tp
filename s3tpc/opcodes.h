#ifndef S3TPC_OPCODES_H_
#define S3TPC_OPCODES_H_


#include <cstdint>


namespace s3tpc {


constexpr const uint16_t NEW_CONNECTION_REQUEST = 0;
constexpr const uint16_t NEW_CONNECTION_ACK = 1;
constexpr const uint16_t NEW_CONNECTION_NACK = 2;

constexpr const uint16_t CLOSE_CONNECTION_REQUEST = 3;
constexpr const uint16_t CLOSE_CONNECTION_ACK = 4;
constexpr const uint16_t CLOSE_CONNECTION_NACK = 5;


}


#endif
