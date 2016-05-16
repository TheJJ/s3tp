#ifndef S3TPC_OPCODES_H_
#define S3TPC_OPCODES_H_


#include <cstdint>


namespace s3tpc {


constexpr const uint16_t REQUEST_OFFSET = 0;
constexpr const uint16_t ACK_OFFSET = 1;
constexpr const uint16_t NACK_OFFSET = 2;

constexpr const uint16_t NEW_CONNECTION_BASE = 0;
constexpr const uint16_t NEW_CONNECTION_REQUEST = NEW_CONNECTION_BASE + REQUEST_OFFSET;
constexpr const uint16_t NEW_CONNECTION_ACK = NEW_CONNECTION_BASE + ACK_OFFSET;
constexpr const uint16_t NEW_CONNECTION_NACK = NEW_CONNECTION_BASE + NACK_OFFSET;

constexpr const uint16_t CLOSE_CONNECTION_BASE = 3;
constexpr const uint16_t CLOSE_CONNECTION_REQUEST = CLOSE_CONNECTION_BASE;
constexpr const uint16_t CLOSE_CONNECTION_ACK = CLOSE_CONNECTION_BASE + ACK_OFFSET;
constexpr const uint16_t CLOSE_CONNECTION_NACK = CLOSE_CONNECTION_BASE + NACK_OFFSET;

constexpr const uint16_t LISTEN_BASE = 6;
constexpr const uint16_t LISTEN_REQUEST = LISTEN_BASE;
constexpr const uint16_t LISTEN_ACK = LISTEN_BASE + ACK_OFFSET;
constexpr const uint16_t LISTEN_NACK = LISTEN_BASE + NACK_OFFSET;

constexpr const uint16_t WAIT_FOR_PEER_BASE = 9;
constexpr const uint16_t WAIT_FOR_PEER_REQUEST = WAIT_FOR_PEER_BASE;
constexpr const uint16_t WAIT_FOR_PEER_ACK = WAIT_FOR_PEER_BASE + ACK_OFFSET;
constexpr const uint16_t WAIT_FOR_PEER_NACK = WAIT_FOR_PEER_BASE + NACK_OFFSET;

constexpr const uint16_t CONNECT_BASE = 12;
constexpr const uint16_t CONNECT_REQUEST = CONNECT_BASE;
constexpr const uint16_t CONNECT_ACK = CONNECT_BASE + ACK_OFFSET;
constexpr const uint16_t CONNECT_NACK = CONNECT_BASE + NACK_OFFSET;


}


#endif
