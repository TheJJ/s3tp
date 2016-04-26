#include <stdio.h>
#include <s3tp.h>

int main() {
	char input[0x100];
	int ret = s3tp_init_with_socket("/tmp/test");
	if (ret == 0) {
		printf("Success\n");
	} else {
		perror("Error");
		return 1;
	}
	fgets(input, sizeof(input), stdin);
	printf("You've entered: %s\n", input);
	s3tp_destroy();
	return 0;
}
