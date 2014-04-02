#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "netting.h"
#include "misc.h"

// TODO: Do poprawienia
net_t *file_to_net( net_t *n, char *filename )
{
	assert( n != NULL );

	// Alokacja pamięci dla wektora
	int el_num = 64;
	n->vec = malloc( el_num * sizeof(int) );
	if( n->vec == NULL )
	{
		fprintf( stderr, "Nie można zaalokować pamięci.\n" );
		exit(EXIT_FAILURE);
	}

	// Odczyt z pliku
	FILE *f = fopen( filename, "r" );
	if( f == NULL )
	{
		fprintf( stderr, "Nie można otworzyć pliku: %s\n", filename );
		exit(EXIT_FAILURE);
	}

	if( fscanf( f, "%d %d", &(n->rows), &(n->cols) ) != 2 )
	{
		fprintf( stderr, "Zły format pliku z siatką.\n" );
		exit(EXIT_FAILURE);
	}

	int x, y;
	int i = 0;
	while( fscanf( f, "%d %d", &x, &y ) == 2 )
	{
		// W razie potrzeby zwiększenie rozmiaru wektora
		if( i == el_num )
		{
			n->vec = realloc( n->vec, 2 * el_num * sizeof(int) );
			if( n->vec == NULL )
			{
				fprintf( stderr, "Nie można zaalokować pamięci.\n" );
				exit(EXIT_FAILURE);
			}
			el_num = 2 * el_num;
		}
		n->vec[i] = x_and_y_to_xy( x, y, n );

		i++;
	}
	n->vec_size = i;

	fclose(f);

	return n;
}

// TODO: Do poprawienia
void net_to_file( net_t *net, char *filename, char *dir )
{
	assert( net != NULL );

	make_dir( dir );
	char *path = create_path( filename, dir );

	// Zapis do pliku
	FILE *f = fopen( path, "w" );
	if( f == NULL )
	{
		fprintf( stderr, "Nie można zapisać do pliku: %s\n", path );
		exit(EXIT_FAILURE);
	}
	free(path);

	fprintf( f, "%d %d\n", net->rows, net->cols );

	for( int i= 0; i < net->vec_size; i++ )
	{
		int x, y;
		xy_to_x_and_y( &x, &y, net->vec[i], net );
		fprintf( f, "%d %d\n", x, y );
	}
	fclose(f); 
}

// NOTE: Do wyrzucenia?
int x_and_y_to_xy( int x, int y, net_t *net )
{
	// Zakładamy że w pliku siatka zaczyna się od punktu (1,1), a tablica od (0,0)
	int xy = ( net->rows * (x - 1) ) + (y - 1);

	return xy;
}

// NOTE: Do wyrzucenia?
void xy_to_x_and_y( int *x, int *y, int xy, net_t *net )
{
	// Zakładamy że w pliku siatka zaczyna się od punktu (1,1), a tablica od (0,0)
	*x = (int) ( xy / net->cols ) + 1;
	*y = (xy % net->rows) + 1;
}

