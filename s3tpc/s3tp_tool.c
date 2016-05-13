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
	printf("Got connection: %d\n", connection);
	int connection2 = s3tp_create();
	printf("Got connection2: %d\n", connection2);
	int closed = s3tp_close(connection);
	printf("Closed connection: %d\n", closed);
	int closed2 = s3tp_close(connection2);
	printf("Closed connection2: %d\n", closed2);

	fgets(input, sizeof(input), stdin);
	printf("You've entered: %s\n", input);
	s3tp_destroy();
	return 0;
}
