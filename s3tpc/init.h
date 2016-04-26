#ifndef S3TPC_INIT_H_
#define S3TPC_INIT_H_

#define S3TPD_SOCKET_PATH "/run/s3tp/control"

int s3tp_init();
int s3tp_init_with_socket(const char *s3tpd_socket_path);

void s3tp_destroy();

#endif
