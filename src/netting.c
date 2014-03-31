#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "netting.h"

// FIXME: Bez duplikatów i sortowanie
net_t *file_to_net( char *filename )
{
	net_t *n = malloc( sizeof(*n) );
	if( n == NULL )
	{
		fprintf( stderr, "Nie można zaalokować pamięci.\n" );
		exit( EXIT_FAILURE );
	}

	// Alokacja pamięci dla wektora
	int el_num = 64;
	n->vec = malloc( el_num * sizeof(n->vec) );
	if( n->vec == NULL )
	{
		fprintf( stderr, "Nie można zaalokować pamięci.\n" );
		exit( EXIT_FAILURE );
	}

	// Odczyt z pliku
	FILE *f = fopen( filename, "r" );
	if( f == NULL )
	{
		fprintf( stderr, "Nie można otworzyć pliku.\n" );
		exit( EXIT_FAILURE );
	}

	if( fscanf( f, "%d %d", &(n->rows), &(n->cols) ) != 2 )
	{
		fprintf( stderr, "\n" );
		exit( EXIT_FAILURE );
	}

	int x, y;
	int i = 0;
	while( fscanf( f, "%d %d", &x, &y ) == 2 )
	{
		// W razie potrzeby zwiększenie rozmiaru wektora
		if( i == el_num )
		{
			n->vec = realloc( n->vec, 2 * el_num * sizeof(n->vec) );
			if( n->vec == NULL )
			{
				fprintf( stderr, "Nie można zaalokować pamięci.\n" );
				exit( EXIT_FAILURE );
			}
			el_num = 2 * el_num;
		}
		n->vec[i] = x_and_y_to_xy( x, y, n );

		i++;
	}
	n->vec_size = i;

	return n;
}

void net_to_file( net_t *net, char *filename, char *dir )
{
	// NOTE: check_dir() ?
	// Sprawdzenie czy istnieje i ewentualne stworzenie katalogu
	struct stat st = { 0 };
	if ( stat( dir, &st ) == -1 )
		mkdir( dir, 0700 );

	// NOTE: make_path() ?
	char path[strlen(filename) + strlen(dir) + 2];
	strcpy( path, dir );
	strcat( path, "/" );
	strcat( path, filename );

	// Zapis do pliku
	FILE *f = fopen( path, "w" );
	if( f == NULL )
	{
		fprintf( stderr, "Nie można zapisać do pliku.\n" );
		exit( EXIT_FAILURE );
	}

	fprintf( f, "%d %d\n", net->rows, net->cols );

	for( int i= 0; i < net->vec_size; i++ )
	{
		
		int x, y;
		xy_to_x_and_y( &x, &y, net->vec[i], net );
		fprintf( f, "%d %d\n", x, y );
	}
	fclose( f ); 
}

int x_and_y_to_xy( int x, int y, net_t *net )
{
	// Zakładamy że w pliku siatka zaczyna się od punktu (1,1), a tablica od (0,0)
	int xy = ( net->rows * (x - 1) ) + (y - 1);

	return xy;
}

void xy_to_x_and_y( int *x, int *y, int xy, net_t *net )
{
	// Zakładamy że w pliku siatka zaczyna się od punktu (1,1), a tablica od (0,0)
	*x = (int) ( xy / net->cols ) + 1;
	*y = (xy % net->rows) + 1;
}

