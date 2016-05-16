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

constexpr const uint16_t LISTEN_REQUEST = 6;
constexpr const uint16_t LISTEN_ACK = 7;
constexpr const uint16_t LISTEN_NACK = 8;

constexpr const uint16_t PEER_CONNECTED = 9;

constexpr const uint16_t CONNECT_REQUEST = 10;
constexpr const uint16_t CONNECT_ACK = 11;
constexpr const uint16_t CONNECT_NACK = 12;


}


#endif
