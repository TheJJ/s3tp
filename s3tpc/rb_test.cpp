#include <iostream>
#include <unistd.h>

#include "ringbuffer.h"


using namespace std;
using namespace s3tpc;


void state(RingBuffer &r) {
	cout << "CAP: " << r.get_capacity() << endl;
	cout << "DAT: " << r.get_available_data() << endl;
	cout << "SPA: " << r.get_available_space() << endl;
}


void do_read(RingBuffer &r, size_t size) {
	char dst[10] = {0};
	size_t ret = r.read(dst, size);
	cout << "\ndo_read: " << ret << endl;
	write(0, dst, size);
	cout << endl;
	state(r);
}


void do_write(RingBuffer &r, size_t offset, size_t size) {
	const char *src = "ABCDEFGHIJ";
	size_t ret = r.write(src + offset, size);
	cout << "\ndo_write: " << ret << endl;
	state(r);
}


int main() {
	RingBuffer r{10};

	do_write(r, 0, 4);
	do_write(r, 4, 2);
	do_read(r, 4);

	do_write(r, 0, 8);
	do_read(r, 10);

	return 0;
}
