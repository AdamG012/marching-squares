#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h> 
#include <assert.h>
#include <pthread.h>
#include "../src/marching_squares_parallel.h"


int verification(uint8_t* c1, uint8_t* c2, int n) {

	int i;
	for (i = 0; i < n; i++) {

		if (c1[i] != c2[i]) {
			return 0;
		}
	}

	return 1;
}


void multithreaded_marching_squares_test(uint8_t* input_image, size_t width, size_t height, uint8_t threshold, uint8_t pos_colour, uint8_t neg_colour, uint8_t* actual, uint8_t* actual_segments, uint8_t* actual_image) {

	// Declare the width, height, threads and data arrays.
	pthread_t threads[N_THREADS];
	struct thread_data data[N_THREADS];

	// SET VALUES OF IMAGE AND READ IT IN 
	uint8_t* new_image = malloc(sizeof(uint8_t) * width * height);

	// Get the amount of rows for each thread
	size_t thread_rows = height/( 3 * N_THREADS );
	
	// Can switch this so that it starts from larger to smaller one instead
	for (int i = 0; i < N_THREADS; i++) {
		data[i].in_width = width;
		data[i].in_height = height;
		data[i].input_image = input_image;
		data[i].new_image = new_image;
		data[i].start_index = i * thread_rows * 3;
		data[i].end_index = (i + 1) * thread_rows * 3;
		data[i].threshold = threshold;

		// If this is the last thread then give its end row as the height
		if (i == N_THREADS - 1) {
			data[i].end_index = height;
		}

		printf("Size of thread row: %ld\n", data[i].end_index);
		printf("Start Index: %ld\n", data[i].start_index);

		data[i].segment_length = ((width * (data[i].end_index - data[i].start_index))/9);
		data[i].segments = calloc(data[i].segment_length * width * height, sizeof(uint8_t));
		data[i].pos_colour = pos_colour;
		data[i].neg_colour = neg_colour;

		pthread_create(threads+i, NULL, worker, data+i);
	}

	// Join all the threads after they have completed.
	for (int i = 0; i < N_THREADS; i++) {
		pthread_join(threads[i], NULL);
		free(data[i].segments);
	}

	/*
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
	msquare_fill(result, width, height, segments, width * height / 9, 0, 1);
	*/

	// Print out result matrix
	for (int i = 0; i < width * height; i++) {
		if (i % width == 0) {
			printf("\n");
		}
		printf("%d ", new_image[i]);
	}

	printf("\nTesting msquare_fill:\n");
	assert (verification(actual_image, new_image, width * height));
	printf("Valid\n");

	free((void*)input_image);
	free((void*)new_image);
	free((void*)actual);
	free((void*)actual_image);
	free((void*)actual_segments);

	return;
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

	multithreaded_marching_squares_test(mata, width, height, 128, 1, 0, actual, actual_segments, actual_image);
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


