#ifndef S3TPC_S3TPC_C_H_
#define S3TPC_S3TPC_C_H_


#ifdef __cplusplus
#include <cstddef>
#include <cstdint>
#else
#include <stddef.h>
#include <stdint.h>
#endif


#ifdef __cplusplus
extern "C" {
#endif


int s3tp_init(const char *socket_path);
void s3tp_destroy();

int s3tp_create();
int s3tp_connect(int connection, uint16_t port);
int s3tp_listen(int connection, uint16_t port);
int s3tp_wait_for_peer(int connection);
int s3tp_send(int connection, const char *data, uint16_t length);
int s3tp_receive(int connection, char *destination, uint16_t length);
int s3tp_close(int connection);

int s3tp_local_port(int connection);
int s3tp_remote_port(int connection);


#ifdef __cplusplus
}
#endif


#endif
