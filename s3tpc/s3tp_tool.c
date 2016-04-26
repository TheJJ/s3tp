#include <stdio.h>
#include <s3tp.h>


int main() {
	int ret = s3tp_init_with_socket("/tmp/test");
	if (ret == 0) {
		printf("Success\n");
	} else {
		perror("Error");
		return 1;
	}
	s3tp_destroy();
	return 0;
}
