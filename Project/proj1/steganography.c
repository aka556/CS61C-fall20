/************************************************************************
**
** NAME:        steganography.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**				Justin Yokota - Starter Code
**				xiaoyu - Final Code
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"

//Determines what color the cell at the given row/col should be. This should not affect Image, and should allocate space for a new Color.
Color *evaluateOnePixel(Image *image, int row, int col)
{
	//YOUR CODE HERE
	uint8_t lsb = image->image[row][col].B & 1;
	Color *newColor = malloc(sizeof(Color));

	// check if malloc succeeded
	if (newColor == NULL) {
		fprintf(stderr, "Memory allocation failed\n");
		return NULL;
	}

	if (lsb == 0) {
		newColor->R = 0;
		newColor->G = 0;
		newColor->B = 0;
	} else {
		newColor->R = 255;
		newColor->G = 255;
		newColor->B = 255;
	}
	return newColor;
}

//Given an image, creates a new image extracting the LSB of the B channel.
Image *steganography(Image *image)
{
	//YOUR CODE HERE
	Image *newImage = malloc(sizeof(Image));
	if (newImage == NULL) {
		fprintf(stderr, "Memory allocation failed\n");
		return NULL;
	}

	newImage->rows = image->rows;
	newImage->cols = image->cols;
	newImage->image = malloc(newImage->rows * sizeof(Color *));

	if (newImage->image == NULL) {
		fprintf(stderr, "Memory allocation failed\n");
		free(newImage);
		return NULL;
	}

	for (uint32_t i = 0; i < newImage->rows; i++) {
		newImage->image[i] = malloc(newImage->cols * sizeof(Color));
		if (newImage->image[i] == NULL) {
			fprintf(stderr, "Memory allocation failed\n");
			for (uint32_t j = 0; j < i; j++) {
				free(newImage->image[j]);
			}
			free(newImage->image);
			free(newImage);
			return NULL;
		}
		for (uint32_t j = 0; j < newImage->cols; j++) {
			Color *newColor = evaluateOnePixel(image, i, j);
			if (newColor == NULL) {
				for (uint32_t k = 0; k <= i; k++) {
					free(newImage->image[k]);
				}
				free(newImage->image);
				free(newImage);
				return NULL;
			}
			newImage->image[i][j] = *newColor;
			free(newColor);
		}
	}
	return newImage;
}

/*
Loads a file of ppm P3 format from a file, and prints to stdout (e.g. with printf) a new image, 
where each pixel is black if the LSB of the B channel is 0, 
and white if the LSB of the B channel is 1.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a file of ppm P3 format (not necessarily with .ppm file extension).
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!
*/
int main(int argc, char **argv)
{
	//YOUR CODE HERE
	if (argc != 2) {
		fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
		exit(-1);
	}

	Image *fp = readData(argv[1]);
	if (fp == NULL) exit(-1);
	
	Image *newImage = steganography(fp);
	if (newImage == NULL) {
		freeImage(fp);
		exit(-1);
	}

	writeData(newImage);
	freeImage(fp);
	freeImage(newImage);
	return 0;
}
