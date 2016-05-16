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
	int connection = s3tp_create();
	printf("\nGot connection: %d\n", connection);
	int connection2 = s3tp_create();
	printf("Got connection2: %d\n", connection2);

	int local_port = s3tp_local_port(connection);
	printf("\nLocal port: %d\n", local_port);
	int remote_port = s3tp_remote_port(connection);
	printf("Remote port: %d\n", remote_port);

	int connected = s3tp_connect(connection, 80);
	printf("\nConnected: %d\n", connected);
	local_port = s3tp_local_port(connection);
	printf("Local port: %d\n", local_port);
	remote_port = s3tp_remote_port(connection);
	printf("Remote port: %d\n", remote_port);

	int connected2 = s3tp_connect(connection2, 80);
	printf("\nConnected2: %d\n", connected2);
	local_port = s3tp_local_port(connection2);
	printf("Local port2: %d\n", local_port);
	remote_port = s3tp_remote_port(connection2);
	printf("Remote port2: %d\n", remote_port);

	int closed = s3tp_close(connection);
	printf("\nClosed connection: %d\n", closed);
	int closed2 = s3tp_close(connection2);
	printf("Closed connection2: %d\n", closed2);

	fgets(input, sizeof(input), stdin);
	printf("You've entered: %s\n", input);
	s3tp_destroy();
	return 0;
}
