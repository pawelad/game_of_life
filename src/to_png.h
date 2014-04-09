#ifndef TO_PNG_H
#define TO_PNG_H

#include <stdint.h>

#include "netting.h"

// Coloured pixel
typedef struct {
	uint8_t red;
	uint8_t green;
	uint8_t blue;
} pixel_t;

// Picture structure
typedef struct  {
	pixel_t *pixels;
	size_t width;
	size_t height;
} bitmap_t;

// Returns pointer to pixel with given coordinates
pixel_t *pixel_at( bitmap_t *bitmap, int x, int y );

// Saving net to PNG file in given scale
void net_to_png( net_t *n, char *file_path, int scale );

// Saving picture struct to PNG file
int save_png_to_file( bitmap_t *bitmap, char *path );

#endif
