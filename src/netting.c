#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <assert.h>

#include "netting.h"
#include "misc.h"

#define RANDOM_NET_SIZE 100

net_t *file_to_net( net_t *n, char *filename )
{
	assert( n != NULL );

	FILE *f = fopen( filename, "r" );
	if( f == NULL )
		print_error_file( filename );

	char line[MAX_LINE];
	int pos;

	// Checking net file format for '<int> <int>\n'
	fgets( line, MAX_LINE, f );
	if( sscanf( line, "%d %d %n", &(n->rows), &(n->cols), &pos ) != 2 )
	{
		print_error("net");
	}
	else if( line[pos] != '\0' )
	{
		print_error("net");
	}

	n->vec = calloc( n->rows * n->cols, sizeof(unsigned char) );
	if( n->vec == NULL )
		print_error("alloc");

	int i, x, y;
	while( fgets( line, MAX_LINE, f ) != NULL )
	{
		// Checking net file format for '<int> <int>\n'
		if( sscanf( line, "%d %d %n", &x, &y, &pos ) != 2 )
		{
			print_error("net");
		}
		else if( line[pos] != '\0' )
		{
			print_error("net");
		}

		if( x > n->rows || y > n->cols )
		{
			fprintf( stderr, "%sERROR:%s Coordinates of living cell in net file is bigger then net dimensions.\n", COLOR_RED, COLOR_RESET );
			exit(EXIT_FAILURE);
		}

		i = ( n->cols * (x - 1) ) + (y - 1);
		n->vec[i] = 1;
	}

	fclose(f);

	return n;
}

net_t *random_net( net_t *n )
{
	assert( n != NULL );

	srand( time(NULL) );

	n->rows = RANDOM_NET_SIZE;
	n->cols = RANDOM_NET_SIZE;

	n->vec = calloc( n->rows * n->cols, sizeof(unsigned char) );
	if( n->vec == NULL )
		print_error("alloc");

	int r;
	for( int i = 0; i < n->rows * n->cols; i++ )
	{
		// 20% chances of cell being alive
		r = rand() % 10;
		if( r < 2 )
			n->vec[i] = 1;
	}

	return n;
}

void net_to_file( net_t *n, char *filename, char *dir )
{
	assert( n != NULL );

	int k = strlen(dir) +
			strlen("/") +
			strlen(filename) + 1;
	char *path = malloc( k );
	if( path == NULL )
		print_error("alloc");
	snprintf( path, k, "%s/%s", dir, filename );

	FILE *f = fopen( path, "w" );
	if( f == NULL )
		print_error_file( filename );

	free(path);

	fprintf( f, "%d %d\n", n->rows, n->cols );

	int x, y;
	for( int i = 0; i < n->rows * n->cols; i++ )
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
