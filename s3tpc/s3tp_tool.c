#include <stdio.h>
#include <stdint.h>
#include <s3tp.h>
#include <unistd.h>


int main() {
	char input[0x100];
	int ret = s3tp_init("/tmp/test");
	if (ret == 0) {
		printf("Success\n");
	} else {
		perror("Error");
		return 1;
	}

	char buffer[100];

	int connection = s3tp_create();
	printf("\n(%d) Got connection\n", connection);
	int connection2 = s3tp_create();
	printf("(%d) Got connection\n", connection2);

	int local_port = s3tp_local_port(connection);
	printf("\n(%d) Local port: %d\n", connection,  local_port);
	int remote_port = s3tp_remote_port(connection);
	printf("(%d) Remote port: %d\n", connection, remote_port);

	int listening = s3tp_listen(connection, 80);
	printf("\n(%d) Listening: %d\n", connection, listening);
	local_port = s3tp_local_port(connection);
	printf("(%d) Local port: %d\n", connection, local_port);
	remote_port = s3tp_remote_port(connection);
	printf("(%d) Remote port: %d\n", connection, remote_port);

	int wait = s3tp_wait_for_peer(connection);
	printf("\n(%d) Waiting for peer: %d\n", connection, wait);
	local_port = s3tp_local_port(connection);
	printf("(%d) Local port: %d\n", connection, local_port);
	remote_port = s3tp_remote_port(connection);
	printf("(%d) Remote port: %d\n", connection, remote_port);

	int connected = s3tp_connect(connection2, 80);
	printf("\n(%d) Connected: %d\n", connection2, connected);
	local_port = s3tp_local_port(connection2);
	printf("(%d) Local port: %d\n", connection2, local_port);
	remote_port = s3tp_remote_port(connection2);
	printf("(%d) Remote port: %d\n", connection2, remote_port);

	int received = s3tp_receive(connection, buffer, 128);
	printf("\n(%d) Received: %d", connection, received);
	if (received > 0) {
		printf(" %s", buffer);
	}
	printf("\n");
	int received2 = s3tp_receive(connection2, buffer, 128);
	printf("\n(%d) Received: %d", connection2, received2);
	if (received2 > 0) {
		printf(" %s", buffer);
	}
	printf("\n");

	int closed = s3tp_close(connection);
	printf("\n(%d) Closed connection: %d\n", connection, closed);
	int closed2 = s3tp_close(connection2);
	printf("(%d) Closed connection2: %d\n", connection2, closed2);

	fgets(input, sizeof(input), stdin);
	printf("You've entered: %s\n", input);
	s3tp_destroy();
	return 0;
}
