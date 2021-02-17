#include <stdio.h>
#include <stdint.h>

/**
* @param input, input gray-scale image, 0x00 to 0xFF
* @param width, width of the image
* @param height, height of the image
* @param threshold, pixel value to generate a 1bit image
* @return uint8_t*, 1bit image of 0x00 or 0xFF with width*height pixels
*/
uint8_t* msquare_1bit(uint8_t* input, size_t in_width, size_t in_height, 
		uint8_t threshold);


/**
* @param data, pixels of the image
* @param width, width of the image
* @param height, height of the image
* @param segments, different segments from 0x00 to 0xFF
* @param segments_length, number of segments inputted
*/
void msquare_detect(uint8_t* data, size_t width, size_t height, uint8_t* segments, size_t segments_length);


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
uint8_t* segments, size_t seg_length, uint8_t pos_colour, uint8_t neg_colour);


