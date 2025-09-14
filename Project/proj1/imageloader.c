/************************************************************************
**
** NAME:        imageloader.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**              Justin Yokota - Starter Code
**				xiaoyu - Final Code
**
**
** DATE:        2020-08-15
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include "imageloader.h"

//Opens a .ppm P3 image file, and constructs an Image object. 
//You may find the function fscanf useful.
//Make sure that you close the file with fclose before returning.
Image *readData(char *filename) 
{
	//YOUR CODE HERE
	// read file
	FILE *fp = fopen(filename, "r");
	// check if file opened successfully
	if (fp == NULL) {
		fprintf(stderr, "Error opening file\n");
		return NULL;
	}

	char magic[3];
	uint32_t rows, cols, maxval;
	
	// read magic number
	if (fscanf(fp, "%2s", magic) != 1) {
		fprintf(stderr, "Error reading magic number\n");
		fclose(fp);
		return NULL;
	}

	if (magic[0] != 'P' || magic[1] != '3') {
		fprintf(stderr, "Invalid magic number, %s\n", magic);
		fclose(fp);
		return NULL;
	}

	if (fscanf(fp, "%u %u", &cols, &rows) != 2) {
		fprintf(stderr, "Error reading image size\n");
		fclose(fp);
		return NULL;
	}

	if (fscanf(fp, "%u", &maxval) != 1 || maxval != 255) {
		fprintf(stderr, "Error reading maxval\n");
		fclose(fp);
		return NULL;
	}

	Image * img = malloc(sizeof(Image));
	img->rows = rows;
	img->cols = cols;
	img->image = malloc(rows * sizeof(Color *));

	// read pixel data by line
	for (uint32_t k = 0; k < rows; k++) {
		img->image[k] = malloc(cols * sizeof(Color));
	}

	// read the pixel one by one
	for (uint32_t i = 0; i < rows; i++) {
		for (uint32_t j = 0; j < cols; j++) {
			int r, g, b;
			if (fscanf(fp, "%d %d %d", &r, &g, &b) != 3) {
				fprintf(stderr, "Error reading pixel data\n");
				// free allocated memory
				// for (uint32_t k = 0; k < rows; k++) {
				// 	free(img->image[k]);
				// }
				// free(img->image);
				// free(img);
				freeImage(img);
				fclose(fp);
				return NULL;
			}
			img->image[i][j].R = (uint8_t)r;
			img->image[i][j].G = (uint8_t)g;
			img->image[i][j].B = (uint8_t)b;
		}
	}
	
	fclose(fp);
	return img;
}

//Given an image, prints to stdout (e.g. with printf) a .ppm P3 file with the image's data.
void writeData(Image *image)
{
	//YOUR CODE HERE
	printf("P3\n");
	printf("%u %u\n", image->cols, image->rows);
	printf("255\n");

	for (uint32_t i = 0; i < image->rows; i++) {
		for (uint32_t j = 0; j < image->cols; j++) {
			printf("%3u %3u %3u", image->image[i][j].R, image->image[i][j].G, image->image[i][j].B);

			if (j < image->cols - 1) {
				printf("   ");
			}
		}
		printf("\n");
	}
}

//Frees an image
void freeImage(Image *image)
{
	//YOUR CODE HERE
	for (uint32_t i = 0; i < image->rows; i++) {
		free(image->image[i]);
	}
	free(image->image);
	free(image);
}