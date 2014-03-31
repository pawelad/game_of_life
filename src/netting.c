#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "netting.h"

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

		// NOTE: Do osobnej funkcji?
		// Zakładamy że w pliku siatka zaczyna się od punktu (1,1), a tablica od (0,0)
		n->vec[i] = ( n->rows * (x - 1) ) + (y - 1);

		i++;
	}
	n->vec_size = i;

	return n;
}

void net_to_file( net_t *net, char *filename, char *dir )
{
	// NOTE: Do oddzielniej funkcji?
	// Sprawdzenie czy istnieje i ewentualne stworzenie katalogu
	struct stat st = { 0 };
	if ( stat( dir, &st ) == -1 )
		mkdir( dir, 0700 );

	// NOTE: Do oddzielnej funkcji?
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

	fprintf( f, "%d %d\n", net->rows, net-> cols );

	for( int i= 0; i < net->vec_size; i++ )
	{
		// NOTE: Do osobnej funkcji?
		// Zakładamy że w pliku siatka zaczyna się od punktu (1,1), a tablica od (0,0)
		int xy = net->vec[i];
		int x = (int) ( xy / net->cols ) + 1;
		int y = (xy % net->rows) + 1;
		fprintf( f, "%d %d\n", x, y );
	}

	fclose( f ); 
}
