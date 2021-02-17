#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h> 
#include <assert.h>
#include "../src/marching_squares.c"

int verification(uint8_t* c1, uint8_t* c2, int n) {

	int i;
	for (i = 0; i < n; i++) {
		if (c1[i] != c2[i]) {
			return 0;
		}
	}
	return 1;
}


size_t test_valid() {
	// Size and width of the image
	size_t width, height;
	uint8_t threshold = 128;

	// Read the width and the height
	if (!scanf("%ld", &width) || !scanf("%ld", &height)) {
		return 1;
	}

	if (width * height < 9) {
		return 1;
	}


	printf("Testing img:%ld by %ld\n", width, height);

	// Create the matrix to read from input
	uint8_t* mata = (uint8_t*)(malloc(width * height * sizeof(uint8_t)));
	uint8_t* actual = (uint8_t*)(calloc(width * height, sizeof(uint8_t)));

	int i = 0;
	int d = 0;
	scanf("%d", &d);
       	threshold = (uint8_t)d;	

	while (i < width * height) {
		scanf("%d", &d);
		mata[i] = (uint8_t)d;
		i += 1;
	}


	i = 0;
	// Read the actual result
	while (i < width * height) {
		scanf("%d", &d);
		actual[i] = (uint8_t)d;
		i += 1;
	}

	i = 0;
	// Allocate size for the valid segments
	uint8_t* actual_segments = malloc(width * height / 9 * sizeof(uint8_t));

	// Read the actual result
	while (i < width * height / 9) {
		scanf("%d", &d);
		actual_segments[i] = (uint8_t)d;
		i += 1;
	}
	
	// Then allocate size for the image result
	uint8_t* actual_image = malloc(width * height * sizeof(uint8_t));

	// Read the new_image result for fill
	i = 0;
	while (i < width * height) {
		scanf("%d", &d);
		actual_image[i] = (uint8_t)d;
		i += 1;
	}
	
	// Create the result 1bit threshold image
	uint8_t* result = msquare_1bit(mata, width, height, threshold);

	// Testing the msquare_1bit method
	printf("\nTesting msquare_1bit:\n");
	assert (verification(actual, result, width * height));
	printf("Valid\n");

	// Allocate the size for the segments and call the detect function on this
	uint8_t* segments = malloc((width * height) / 9 * sizeof(uint8_t));
	msquare_detect(result, width, height, segments, width * height / 9);

	// Testing the msquare_detect method
	printf("\nTesting msquare_detect:\n");
	assert (verification(actual_segments, segments, width * height/9));
	printf("Valid\n");

	// Call the msquare fill on the result image and the segments
	msquare_fill(result, width, height, segments, width * height / 9, 1, 0);

	printf("\nTesting msquare_fill:\n");
	assert (verification(actual_image, result, width * height));
	printf("Valid\n");
	
	// Print out result matrix
	for (int i = 0; i < width * height; i++) {
		if (i % width == 0) {
			printf("\n");
		}
		printf("%d ", result[i]);
	}

	// To print out the segments
	for (int i = 0; i < width * height / 9; i++) {
		if (i % width/3 == 0) {
			printf("\n");
		}
		printf("%d ", segments[i]);
	}

	free((void*)mata);
	free((void*)result);
	free((void*)segments);
	free((void*)actual_segments);

	return 0;
}

int main(int argc, char** argv) {

	if (test_valid()) {
		printf("Invalid usage\n");
		return 1;
	}

	printf("\nSuccess\n");
	exit(EXIT_SUCCESS);

}
