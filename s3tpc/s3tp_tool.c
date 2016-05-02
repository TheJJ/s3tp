#include <stdio.h>
#include <stdint.h>
#include <s3tp.h>
#include <unistd.h>


/*
void do_insert(struct hash_map *map, unsigned key, int32_t value) {
	printf("\ndo_insert %u %d\n", key, value);
	int32_t *value_ptr = malloc(sizeof(int32_t));
	*value_ptr = value;
	int32_t *old_value = hm_insert(map, key, value_ptr);
	if (old_value == NULL) {
		printf("old: null\n");
	} else {
		printf("old: %d\n", *old_value);
	}
	printf("size: %lu\n", hm_size(map));
}


void do_find(struct hash_map *map, unsigned key) {
	printf("\ndo_find %u\n", key);
	int32_t *ptr = hm_find(map, key);
	if (ptr == NULL) {
		printf("value: null\n");
	} else {
		printf("value: %d\n", *ptr);
	}
	printf("size: %lu\n", hm_size(map));
}

void do_remove(struct hash_map *map, unsigned key) {
	printf("\ndo_remove %u\n", key);
	int32_t *ptr = hm_remove(map, key);
	if (ptr == NULL) {
		printf("value: null\n");
	} else {
		printf("value: %d\n", *ptr);
	}
	printf("size: %lu\n", hm_size(map));
}


int main() {
	struct hash_map *map = hm_create(10);
	do_insert(map, 10, 20);
	do_insert(map, 20, 30);
	do_insert(map, 10, 30);
	do_insert(map, 10, 40);
	do_find(map, 10);
	do_find(map, 20);
	do_find(map, 5);
	do_find(map, 30);
	do_remove(map, 10);
	do_remove(map, 5);
	do_remove(map, 20);
	return 0;
}*/

/*
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
*/


int main() {
	char input[0x100];
	int ret = s3tp_init("/tmp/test");
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
