#ifndef S3TPC_S3TPC_C_H_
#define S3TPC_S3TPC_C_H_


#ifdef __cplusplus
extern "C" {
#endif

int s3tp_init(const char *socket_path);
void s3tp_destroy();

#ifdef __cplusplus
}
#endif


#endif
