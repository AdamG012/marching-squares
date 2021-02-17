#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include "marching_squares_parallel.h"


void msquare_1bit(uint8_t* input, size_t in_width, size_t in_height, uint8_t threshold, uint8_t* new_image, size_t start_index, size_t end_index) {

	if (input == NULL || threshold < 0 || new_image == NULL) {
		return;
	}

	// Loop through all the ints in the array
	for (size_t i = start_index; i < end_index; i++) {
		// Loop through all the columns
		for (size_t j = 0; j < in_width; j++) {
			// Less than 128 than give it 0 else give it 0xFF
			new_image[(i * in_width) + j] = (input[(i * in_width) + j] <= threshold) ? 0x00 : 0xFF;
		}
	}

	return;
}
 

void msquare_detect(uint8_t* data, size_t width, size_t height, uint8_t* segments, size_t segments_length, size_t start_index, size_t end_index) {

	// Check if the two arrays of unsigned ints are valid
	if (data == NULL || segments == NULL) {
		return;
	}

	register size_t n = 0;

	// Loop in a 3 by 3 grid
	for (size_t i = start_index + 1; i < end_index - 1; i += 3) {
		for (size_t j = 1; j < width - 1; j += 3) {
			
			register uint8_t cell_value = 0;
			// TODO use of loop unrolling here to avoid loop overhead
			cell_value += (!data[((i - 1) * width) + (j - 1)]) ? 8 : 0;
			cell_value += (!data[((i - 1) * width) + (j + 1)]) ? 4 : 0;
			cell_value += (!data[((i + 1) * width) + (j + 1)]) ? 2 : 0;
			cell_value += (!data[((i + 1) * width) + (j - 1)]) ? 1 : 0;

			// Set the value of the segment to the blurred cell value
			segments[n++] = cell_value;
		}
	}
}


void msquare_fill(uint8_t* new_image, size_t width, size_t height,
uint8_t* segments, size_t seg_length, uint8_t pos_colour, uint8_t neg_colour,
size_t start_index, size_t end_index, uint8_t** matrix_map) {

	// Check if the two arrays of unsigned 8 bit ints are valid
	if (new_image == NULL || segments == NULL) {
		return;
	}


	// Register values for index nth segment, i and j values of matrix
	register size_t n = 0;
	register size_t i = start_index;
	register size_t j = 0;

	// While we havent reached the bottom
	while (i < end_index) {
		// Get the array of new matrix values for this
		const uint8_t* arr = matrix_map[segments[n++]];

		// Then for the next 3 elements set them to either positive or negative colour 
		new_image[(i * width) + j] = (arr[3 * (i % 3)]) ? pos_colour : neg_colour;
		new_image[(i * width) + j + 1] = (arr[3 * (i % 3) + 1]) ? pos_colour : neg_colour;
		new_image[(i * width) + j + 2] = (arr[3 * (i % 3) + 2]) ? pos_colour : neg_colour;

		// Increment the j to 3 cells across
		j += 3;

		// If the value of j has reached the length of the width jump down a row
		if (j > width - 3) {
			j = 0;
			n = 0;
			i++;
		}
	}
	
}


void* worker(void* data) {
	struct thread_data* tdata = (struct thread_data *)data;

	// Call 1bit method
	msquare_1bit(tdata->input_image, tdata->in_width, tdata->in_height, tdata->threshold, tdata->new_image,	tdata->start_index, tdata->end_index);

	// Call the detect method
	msquare_detect(tdata->new_image, tdata->in_width, tdata->in_height, tdata->segments, tdata->segment_length, tdata->start_index, tdata->end_index);

	// Call the fill method
	msquare_fill(tdata->new_image, tdata->in_width, tdata->in_height,
			tdata->segments, tdata->segment_length, 
			tdata->pos_colour, tdata->neg_colour,
			tdata->start_index, tdata->end_index, tdata->matrix_map);

	return NULL;
}

uint8_t** get_matrix_map() {
	const uint8_t matrix_values[16][9] = 
	{{1,1,1,
	1,1,1,
	1,1,1},
	{1,1,1,
	0,1,1,
	0,0,1,},
	{1,1,1,
	1,1,0,
	1,0,0},
	{1,1,1,
	0,0,0,
	0,0,0},
	{1,0,0,
	1,1,0,
	1,1,1},
	{1,1,0,
	1,0,1,
	0,1,1},
	{1,0,0,
	1,0,0,
	1,0,0},
	{1,1,0,
	1,0,0,
	0,0,0},
	{0,0,1,
	0,1,1,
	1,1,1},
	{0,0,1,
	0,0,1,
	0,0,1},
	{0,1,1,
	1,0,1,
	1,1,0},
	{0,1,1,
	0,0,1,
	0,0,0},
	{0,0,0,
	0,0,0,
	1,1,1},
	{0,0,0,
	0,0,1,
	0,1,1},
	{0,0,0,
	1,0,0,
	1,1,0},
	{0,0,0,
	0,0,0,
	0,0,0}};

	uint8_t** const matrix_map = malloc(sizeof(uint8_t*) * 16);
	for (int i = 0; i < 16; i++) {
		matrix_map[i] = malloc(sizeof(uint8_t) * 9);
		matrix_map[i][0] = matrix_values[i][0];
		matrix_map[i][1] = matrix_values[i][1];
		matrix_map[i][2] = matrix_values[i][2];
		matrix_map[i][3] = matrix_values[i][3];
		matrix_map[i][4] = matrix_values[i][4];
		matrix_map[i][5] = matrix_values[i][5];
		matrix_map[i][6] = matrix_values[i][6];
		matrix_map[i][7] = matrix_values[i][7];
		matrix_map[i][8] = matrix_values[i][8];
	}

	return matrix_map;
}

void multithreaded_marching_squares(uint8_t* input_image, size_t width, size_t height, uint8_t threshold, uint8_t pos_colour, uint8_t neg_colour) {
	
	uint8_t** matrix_map = get_matrix_map();

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
		data[i].matrix_map = matrix_map;

		// If this is the last thread then give its end row as the height
		if (i == N_THREADS - 1) {
			data[i].end_index = height;
		}

		data[i].segment_length = ((width/3 * (data[i].end_index - data[i].start_index)/3)) + 9;
		data[i].segments = malloc(data[i].segment_length * sizeof(uint8_t) * width);
		data[i].pos_colour = pos_colour;
		data[i].neg_colour = neg_colour;

		pthread_create(threads+i, NULL, worker, data+i);
	}

	// Join all the threads after they have completed.
	for (int i = 0; i < N_THREADS; i++) {
		pthread_join(threads[i], NULL);
		free(data[i].segments);
	}
	
	free(matrix_map);
	free(new_image);
}
