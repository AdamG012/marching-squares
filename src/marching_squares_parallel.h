#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define N_THREADS (8)

/**
* @param input, input gray-scale image, 0x00 to 0xFF
* @param width, width of the image
* @param height, height of the image
* @param threshold, pixel value to generate a 1bit image
* @return uint8_t*, 1bit image of 0x00 or 0xFF with width*height pixels
*/
void msquare_1bit(uint8_t* input, size_t in_width, size_t in_height, 
		uint8_t threshold, uint8_t* new_image, size_t start_index, size_t end_index);


/**
* @param data, pixels of the image
* @param width, width of the image
* @param height, height of the image
* @param segments, different segments from 0x00 to 0xFF
* @param segments_length, number of segments inputted
*/
void msquare_detect(uint8_t* data, size_t width, size_t height, uint8_t* segments, size_t segments_length, size_t start_index, size_t end_index);


/**
* @param original,
* @param width, width of the image
* @param height, height of the image
* @param segments, the encoded segments 0000 -> 1111
* @param seg_length, number of segments
* @param pos_colour, replace 0xff with positive colour
* @param neg_colour, replace 0x00 with negative colour
*/
void msquare_fill(uint8_t* new_image, size_t width, size_t height,
uint8_t* segments, size_t seg_length, uint8_t pos_colour, uint8_t neg_colour, size_t start_index, size_t end_index, uint8_t** matrix_map);


/**
* @param input_image, the input image
* @param width, width of the image
* @param height, height of the image
* @param threshold, the threshold value to convert to positive or negative
* @param pos_colour, replace 0xff with positive colour
* @param neg_colour, replace 0x00 with negative colour
*/
void multithreaded_marching_squares(uint8_t* input_image, size_t width, size_t height, uint8_t threshold, uint8_t pos_colour, uint8_t neg_colour);


/**
 * The worker thread method that will call all the msquare methods
 * @param data, the thread_data struct for that thread
 *
 */ 
void* worker(void* data);

struct thread_data {
	uint8_t* input_image;
	size_t in_width;
	size_t in_height;
	uint8_t threshold;
	uint8_t* new_image;
	size_t start_index;
	size_t end_index;
	uint8_t pos_colour;
	uint8_t neg_colour;
	uint8_t* segments;
	size_t segment_length;
	uint8_t** matrix_map;
};
