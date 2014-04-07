#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "netting.h"
#include "misc.h"

net_t *file_to_net( net_t *n, char *filename )
{
	assert( n != NULL );

	FILE *f = fopen( filename, "r" );
	if( f == NULL )
	{
		fprintf( stderr, "Can't open file: %s\n", filename );
		exit(EXIT_FAILURE);
	}

	if( fscanf( f, "%d %d", &(n->rows), &(n->cols) ) != 2 )
	{
		fprintf( stderr, "Incorrect net file.\n" );
		exit(EXIT_FAILURE);
	}

	n->vec = calloc( n->rows * n->cols, sizeof(unsigned char) );
	if( n->vec == NULL )
	{
		fprintf( stderr, "Can't allocate memory.\n" );
		exit(EXIT_FAILURE);
	}

	int i, x, y;
	while( fscanf( f, "%d %d", &x, &y ) == 2 )
	{
		if( x > n->rows || y > n->cols )
		{
			fprintf( stderr, "Coordinates in net file are bigger then net dimensions.\n" );
			exit(EXIT_FAILURE);
		}

		i = ( n->cols * (x - 1) ) + (y - 1);
		n->vec[i] = 1;
	}

	fclose(f);

	return n;
}

void net_to_file( net_t *n, char *filename, char *dir )
{
	assert( n != NULL );

	int k = strlen(dir) +
			strlen("/") +
			strlen(filename) + 1;
	char *path = malloc( k );
	snprintf( path, k, "%s/%s", dir, filename );

	FILE *f = fopen( path, "w" );
	if( f == NULL )
	{
		fprintf( stderr, "Can't write to file: %s\n", path );
		exit(EXIT_FAILURE);
	}
	free(path);

	fprintf( f, "%d %d\n", n->rows, n->cols );

	int x, y;
	for( int i= 0; i < n->rows * n->cols; i++ )
	{
		if( n->vec[i] == 1 )
		{
			x = (int) (i / n->cols) + 1;
			y = (i % n->cols) + 1;
			fprintf( f, "%d %d\n", x, y );
		}
	}
	
	fclose(f); 
}
