#include <stdio.h>
#include <stdlib.h>
#include "../src/marching_squares_parallel.h"
#include "blob1.c"
#include "blob2.c"

void test_blob1() {
	
	multithreaded_marching_squares(((uint8_t*)gimp_image_blob1.pixel_data), gimp_image_blob1.width, gimp_image_blob1.height, 128, 1, 0);

	return;

}


void test_blob2() {
	
	multithreaded_marching_squares(((uint8_t*)gimp_image_blob2.pixel_data), gimp_image_blob2.width, gimp_image_blob2.height, 128, 1, 0);

	return;
}

int main(int argc, char** argv) {

	if (argc != 2) {
		return 1;
	}

	int base = 10;
	char* endptr;

	size_t method = strtol(argv[1], &endptr, base);

	if (method == 1) {
		test_blob1();
	} else {
		test_blob2();
	}
}

