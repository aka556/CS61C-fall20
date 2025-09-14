/************************************************************************
**
** NAME:        gameoflife.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Justin Yokota - Starter Code
**				xiaoyu - finish the code
**
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"

//Determines what color the cell at the given row/col should be. This function allocates space for a new Color.
//Note that you will need to read the eight neighbors of the cell in question. The grid "wraps", so we treat the top row as adjacent to the bottom row
//and the left column as adjacent to the right column.
Color *evaluateOneCell(Image *image, int row, int col, uint32_t rule)
{
	//YOUR CODE HERE
	uint32_t liveNeighbors = 0;
	int rows = image->rows;
	int cols = image->cols;

	// current cell state
	Color cell = image->image[row][col];
	int isAlive = cell.R || cell.G || cell.B;

	// check all 8 neighbors
	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			if (i == 0 && j == 0) continue; // skip the cell itself
			int neighborRow = (row + i + rows) % rows; // wrap around
			int neighborCol = (col + j + cols) % cols; // wrap around
			Color neighbor = image->image[neighborRow][neighborCol];
			if (neighbor.R || neighbor.G || neighbor.B) {
				liveNeighbors++;
			}
		}
	}

	// check the state
	int newAlive;
	if (isAlive) {
		newAlive = (rule >> (liveNeighbors + 9)) & 1;
	} else {
		newAlive = (rule >> liveNeighbors) & 1;
	}

	// allocate new color
	Color *newColor = malloc(sizeof(Color));
	if (newColor == NULL) {
		fprintf(stderr, "Memory allocation failed\n");
		exit(-1);
	}

	if (newAlive) {
		newColor->R = 255;
		newColor->G = 255;
		newColor->B = 255;
	} else {
		newColor->R = 0;
		newColor->G = 0;
		newColor->B = 0;
	}
	return newColor;
}

//The main body of Life; given an image and a rule, computes one iteration of the Game of Life.
//You should be able to copy most of this from steganography.c
Image *life(Image *image, uint32_t rule)
{
	//YOUR CODE HERE
	Image *newImage = malloc(sizeof(Image));
	if (newImage == NULL) {
		fprintf(stderr, "Memory allocation failed\n");
		exit(-1);
	}

	newImage->rows = image->rows;
	newImage->cols = image->cols;
	newImage->image = malloc(newImage->rows * sizeof(Color *));
	if (newImage->image == NULL) {
		fprintf(stderr, "Memory allocation failed\n");
		free(newImage);
		exit(-1);
	}

	for (uint32_t i = 0; i < newImage->rows; i++) {
		newImage->image[i] = malloc(newImage->cols * sizeof(Color));
		if (newImage->image[i] == NULL) {
			fprintf(stderr, "Memory allocation failed\n");
			for (uint32_t k = 0; k < i; k++) {
				free(newImage->image[k]);
			}
			free(newImage->image);
			free(newImage);
			exit(-1);
		}
		for (uint32_t j = 0; j < newImage->cols; j++) {
			Color *newColor = evaluateOneCell(image, i, j, rule);
			if (newColor == NULL) {
				for (uint32_t k = 0; k <= i; k++) {
					free(newImage->image[k]);
				}
				free(newImage->image);
				free(newImage);
				exit(-1);
			}
			newImage->image[i][j] = *newColor;
			free(newColor);
		}
	}
	return newImage;
}

/*
Loads a .ppm from a file, computes the next iteration of the game of life, then prints to stdout the new image.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a .ppm.
argv[2] should contain a hexadecimal number (such as 0x1808). Note that this will be a string.
You may find the function strtol useful for this conversion.
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!

You may find it useful to copy the code from steganography.c, to start.
*/
int main(int argc, char **argv)
{
	//YOUR CODE HERE
	if (argc != 3) {
		printf("usage: ./gameOfLife filename rule\n");
		printf("filename is an ASCII PPM file (type P3) with maximum value 255.\n");
		printf("rule is a hex number beginning with 0x; Life is 0x1808.\n");
		exit(-1);
	}

	char *filename = argv[1];
	char *ruleStr = argv[2];

	// check the filename is .ppm
	// if (filename == NULL || strlen(filename) < 4 || strcmp(filename + strlen(filename) - 4, ".ppm") != 0) {
	// 	fprintf(stderr, "Invalid filename. Must end with .ppm\n");
	// 	exit(-1);
	// }

	// if (ruleStr == NULL || strlen(ruleStr) < 3 || ruleStr[0] != '0' || ruleStr[1] != 'x') {
	// 	fprintf(stderr, "Invalid rule format. Must be a hex number starting with 0x\n");
	// 	exit(-1);
	// }

	Image *fp = readData(filename);
	if (fp == NULL) exit(-1);

	uint32_t rule = (uint32_t)strtol(ruleStr, NULL, 16);
	Image *newImage = life(fp, rule);
	if (newImage == NULL) {
		freeImage(fp);
		exit(-1);
	}

	writeData(newImage);
	freeImage(fp);
	freeImage(newImage);
	return 0;
}
