#ifndef S3TPC_DISPATCHER_H_
#define S3TPC_DISPATCHER_H_

extern int is_running;

int dispatch_loop(int control_socket, int event_fd);

#endif
