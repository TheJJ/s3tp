#ifndef S3TPC_S3TPC_C_H_
#define S3TPC_S3TPC_C_H_


#ifdef __cplusplus
#include <cstddef>
#else
#include <stddef.h>
#endif


#ifdef __cplusplus
extern "C" {
#endif


int s3tp_init(const char *socket_path);
void s3tp_destroy();

int s3tp_create();
int s3tp_send(int connection, const char *data, size_t length);
int s3tp_receive(int connection, char *data, size_t length);
int s3tp_close(int connection);


#ifdef __cplusplus
}
#endif


#endif
