#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "marching_squares.h"


uint8_t* msquare_1bit(uint8_t* input, size_t in_width, size_t in_height, 
		uint8_t threshold) {
	// Check if the two arrays of unsiged int 8 bits are valid
	if (input == NULL || threshold <= 0) {
		return NULL;
	}

	// Create the new image
	uint8_t* new_image = (uint8_t*)malloc(in_width * in_height * sizeof(uint8_t));

	// Loop through all the ints in the array
	for (size_t i = 0; i < in_height; i++) {
		// Loop through all the columns
		for (size_t j = 0; j < in_width; j++) {
			// Less than 128 than give it 0 else give it 0xFF
			new_image[(i * in_width) + j] = (input[(i * in_width) + j] <= threshold) ? 0x00 : 0xFF;
		}
	}

	return new_image;
}


void msquare_detect(uint8_t* data, size_t width, size_t height, uint8_t* segments, size_t segments_length) {

	// Check if the two arrays of unsigned ints are valid
	if (data == NULL || segments == NULL) {
		return;
	}

	register size_t n = 0;
	// Loop in a 3 by 3 grid
	for (size_t i = 1; i < height - 1; i += 3) {
		for (size_t j = 1; j < width - 1; j += 3) {
			
			register uint8_t cell_value = 0;
			// TODO use of loop unrolling here to avoid loop overhead
			cell_value += (!data[((i - 1) * height) + (j - 1)]) ? 8 : 0;
			cell_value += (!data[((i - 1) * height) + (j + 1)]) ? 4 : 0;
			cell_value += (!data[((i + 1) * height) + (j + 1)]) ? 2 : 0;
			cell_value += (!data[((i + 1) * height) + (j - 1)]) ? 1 : 0;

			// Set the value of the segment to the blurred cell value
			segments[n++] = cell_value;
		}
	}
}


void msquare_fill(uint8_t* new_image, size_t width, size_t height,
uint8_t* segments, size_t seg_length, uint8_t pos_colour, uint8_t neg_colour) {

	// Matrix values for the segments
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


	// Check if the two arrays of unsigned 8 bit ints are valid
	if (new_image == NULL || segments == NULL) {
		return;
	}

	// Register values for index nth segment, i and j values of matrix
	register size_t n = 0;
	register size_t i = 0;
	register size_t j = 0;
	// While we havent reached the bottom
	while (i < height) {

		// Get the value of the nth segment
		uint8_t value = segments[n++];
		// Get the array of new matrix values for this
		const uint8_t* arr = matrix_values[value];

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

