#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include <png.h>

#include "to_png.h"
#include "misc.h"

pixel_t *pixel_at( bitmap_t *bitmap, int x, int y )
{
	return bitmap->pixels + bitmap->width * y + x;
}

void net_to_png( net_t *n, char *file_path )
{
	assert( n != NULL );

	bitmap_t pic;

	pic.width = n->cols * 2;
	pic.height = n->rows * 2;

	pic.pixels = calloc( sizeof(pixel_t), pic.width * pic.height );

	int i;
	unsigned int y;
	unsigned int x;
	int x1;
	int y1 = 0;
	pixel_t *pixel;

	// One 'dot' is 2 pixel x 2 pixel
	for( y = 0; y < pic.height; y+=2 )
	{
		x1 = 0;
		for( x = 0; x < pic.width; x+=2 )
		{
			i = ( n->cols * y1 + x1 );
			if( n->vec[i] != 1 )
			{
				pixel = pixel_at( &pic, x, y );
				pixel->red = 255;
				pixel->green = 255;
				pixel->blue = 255;

				pixel = pixel_at( &pic, x + 1, y );
				pixel->red = 255;
				pixel->green = 255;
				pixel->blue = 255;

				pixel = pixel_at( &pic, x, y + 1 );
				pixel->red = 255;
				pixel->green = 255;
				pixel->blue = 255;

				pixel = pixel_at( &pic, x + 1, y + 1 );
				pixel->red = 255;
				pixel->green = 255;
				pixel->blue = 255;
			}
			x1++;
		}
		y1++;
	}

	save_png_to_file( &pic, file_path );

	free(pic.pixels);
}

// Based on http://www.lemoda.n/c/write-png/
int save_png_to_file( bitmap_t *bitmap, char *path )
{
	FILE *fp;
	size_t x, y;
	png_structp png_ptr      = NULL;
	png_infop info_ptr       = NULL;
	png_byte ** row_pointers = NULL;
	/* "status" contains the return value of this function. At first
		 it is set to a value which means 'failure'. When the routine
		 has finished its work, it is set to a value which means
		 'success'. */
	int status = -1;
	/* The following number is set by trial and error only. I cannot
		 see where it it is documented in the libpng manual.
	*/
	int pixel_size = 3;
	int depth = 8;
	
	fp = fopen( path, "wb" );
	if( !fp )
	{
		goto fopen_failed;
	}

	png_ptr = png_create_write_struct( PNG_LIBPNG_VER_STRING, NULL, NULL, NULL );
	if( png_ptr == NULL )
	{
			goto png_create_write_struct_failed;
	}

	info_ptr = png_create_info_struct( png_ptr );
	if( info_ptr == NULL )
	{
			goto png_create_info_struct_failed;
	}
	
	/* Set up error handling. */
	if( setjmp( png_jmpbuf(png_ptr) ) )
	{
			goto png_failure;
	}

	/* Set image attributes. */
	png_set_IHDR (png_ptr,
					info_ptr,
					bitmap->width,
					bitmap->height,
					depth,
					PNG_COLOR_TYPE_RGB,
					PNG_INTERLACE_NONE,
					PNG_COMPRESSION_TYPE_DEFAULT,
					PNG_FILTER_TYPE_DEFAULT);

	/* Initialize rows of PNG. */
	row_pointers = png_malloc( png_ptr, bitmap->height * sizeof(png_byte *) );
	for( y = 0; y < bitmap->height; ++y )
	{
		png_byte *row = png_malloc( png_ptr, sizeof(uint8_t) * bitmap->width * pixel_size );
		row_pointers[y] = row;
		for( x = 0; x < bitmap->width; ++x )
		{
			pixel_t *pixel = pixel_at( bitmap, x, y );
			*row++ = pixel->red;
	 		*row++ = pixel->green;
			*row++ = pixel->blue;
		}
	}

	/* Write the image data to "fp". */
	png_init_io( png_ptr, fp );
	png_set_rows( png_ptr, info_ptr, row_pointers );
	png_write_png( png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL );

	/* The routine has successfully written the file, so we set
		 "status" to a value which indicates success. */
	status = 0;

	for( y = 0; y < bitmap->height; y++ )
	{
		png_free( png_ptr, row_pointers[y] );
	}
	png_free( png_ptr, row_pointers );

png_failure:
	png_create_info_struct_failed:
		png_destroy_write_struct (&png_ptr, &info_ptr);
	png_create_write_struct_failed:
		fclose (fp);
	fopen_failed:
		return status;
}