#include <stdio.h>
#include <s3tp.h>
#include <unistd.h>


void state(struct ring_buffer *buffer) {
	printf("CAP: %ld\n", rb_capacity(buffer));
	printf("DAT: %ld\n", rb_available_data(buffer));
	printf("SPA: %ld\n", rb_available_space(buffer));
}

void do_read(struct ring_buffer *buffer, size_t size) {
	char dst[10] = {0};
	ssize_t ret = rb_read(buffer, dst, size);
	printf("\ndo_read: %ld\n", ret);
	write(0, dst, size);
	printf("\n");
	state(buffer);
}

void do_write(struct ring_buffer *buffer, size_t offset, size_t size) {
	const char *src = "ABCDEFGHIJ";
	ssize_t ret = rb_write(buffer, src + offset, size);
	printf("\ndo_write: %ld\n", ret);
	state(buffer);
}

int main() {
	char dst[10] = {0};
	struct ring_buffer *buffer = rb_create(10);

	do_write(buffer, 0, 4);
	do_write(buffer, 4, 2);
	do_read(buffer, 4);

	do_write(buffer, 0, 8);
	do_read(buffer, 10);

	return 0;
}

/*
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
*/
