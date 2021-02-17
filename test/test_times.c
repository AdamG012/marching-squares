#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "../src/marching_squares.h"

int main(int argc, char** argv) {
	
	if (argc != 3) {
		fprintf(stderr, "Usage: ./test_times width height\n");
		exit(EXIT_FAILURE);
	}

	int base = 10;
	char* endptr;

	// Get the width and height from command line arguments
	size_t width = strtol(argv[1], &endptr, base);
	size_t height = strtol(argv[2], &endptr, base);

	
	// Check if the width and height are valid
	if (width < 3 || height < 3) {
		printf("Invalid width or height\n");
		return 1;
	}
	
	uint8_t* image = malloc(width * height * sizeof(uint8_t));
	uint8_t* new_image = msquare_1bit(image, width, height, 128);
	uint8_t* segments = malloc(sizeof(uint8_t) * (width * height)/9);
	size_t segment_length = (width * height)/9;

	msquare_detect(new_image, width, height, segments, segment_length);

	msquare_fill(new_image, width, height, segments, segment_length, 1, 0);

	free(image);
	free(segments);
	free(new_image);
	exit(EXIT_SUCCESS);

}
