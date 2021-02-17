#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include "../src/marching_squares_parallel.h"


int main(int argc, char** argv) {
	
	// Width and height 
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

	// Allocate the size for the image provided
	uint8_t* input_image = calloc(width * height, sizeof(uint8_t));

	// Call the multithreaded message
	multithreaded_marching_squares(input_image, width, height, 128, 1, 0);

	return 0;
}
