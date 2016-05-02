#ifndef S3TPC_DISPATCHER_H_
#define S3TPC_DISPATCHER_H_


#include "hashmap.h"


extern int notification_fd;
extern int is_running;


int dispatch_loop(int control_socket);
int notify_dispatcher();


#endif
