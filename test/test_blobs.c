#include <stdio.h>
#include <stdlib.h>
#include "../src/marching_squares.h"
#include "blob1.c"
#include "blob2.c"

void test_blob1() {
	
	uint8_t* new_image = msquare_1bit((uint8_t*)(gimp_image_blob1.pixel_data), gimp_image_blob1.width, gimp_image_blob1.height, 128);

	uint8_t* segments = malloc(sizeof(uint8_t) * (gimp_image_blob1.width * gimp_image_blob1.height)/9);

	msquare_detect(new_image, gimp_image_blob1.width, gimp_image_blob1.height, segments, (gimp_image_blob1.width * gimp_image_blob1.height)/9);

	msquare_fill(new_image, gimp_image_blob1.width, gimp_image_blob1.height, segments, (gimp_image_blob1.width * gimp_image_blob1.height)/9, 1, 0);

	return;

}


void test_blob2() {
	
	// Get the image with the values set to 0x00 or 0xFF
	uint8_t* new_image = msquare_1bit((uint8_t*)(gimp_image_blob2.pixel_data), gimp_image_blob2.width, gimp_image_blob2.height, 128);

	// Allocate memory for the segments
	uint8_t* segments = malloc(sizeof(uint8_t) * (gimp_image_blob1.width * gimp_image_blob1.height)/9);

	// Call the detects method to find segments 
	msquare_detect(new_image, gimp_image_blob2.width, gimp_image_blob2.height, segments, (gimp_image_blob2.width * gimp_image_blob2.height)/9);

	// Fill the image out by using the segments to blur out selected corners
	msquare_fill(new_image, gimp_image_blob2.width, gimp_image_blob2.height, segments, (gimp_image_blob2.width * gimp_image_blob2.height)/9, 1, 0);

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

