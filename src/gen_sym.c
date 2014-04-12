#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "gen_sym.h"
#include "misc.h"

net_t *sym_gen( net_t *n, rules_t *r )
{
	assert( r != NULL );

	net_t *next_net = malloc( sizeof(net_t) );
	if( next_net == NULL )
		print_error("alloc");

	next_net->cols = n->cols;
	next_net->rows = n->rows;

	next_net->vec = calloc( next_net->rows * next_net->cols, sizeof(unsigned char) );
	if( n->vec == NULL )
		print_error("alloc");
	
	for( int i = 0; i < next_net->rows * next_net->cols; i++ )
	{	
		next_net->vec[i] = next_state( n, i, r );
	}

	free(n->vec);
	free(n);

	return next_net;
}

int next_state( net_t *n, int cell_num, rules_t *r )
{
	int nb = cell_neighborhood( n, cell_num );

	if( n->vec[cell_num] == 1 )
	{
		for( int i = 0; i < r->lives_size; i++ )
		{
			if( nb == r->lives[i] )
				return 1;
		}

		return 0;
	}
	else
	{
		for( int i = 0; i < r->born_size; i++ )
		{
			if( nb == r->born[i] )
				return 1;
		}

		return 0;
	}
}

int cell_neighborhood( net_t *n, int cell_num )
{
	int row = (int) (cell_num / n->cols);
	int col = cell_num % n->cols;
	int k = 0;

	if( row != 0 )
	{
		if( n->vec[ cell_num - n->cols] == 1 )
			k++;

		if( col != 0 )
		{
			if( n->vec[ cell_num - n->cols - 1] == 1 )
				k++;
		}

		if( col != n->cols - 1 )
		{
			if( n->vec[ cell_num - n->cols + 1] == 1 )
				k++;
		}
	}

	if( col != 0 )
	{
		if( n->vec[ cell_num - 1] == 1 )
			k++;
	}

	if( col != n->cols - 1 )
	{
		if( n->vec[ cell_num + 1] == 1 )
			k++;
	}

	if( row != n->rows - 1 )
	{
		if( n->vec[ cell_num + n->cols] == 1 )
			k++;

		if( col != 0 )
		{
			if( n->vec[ cell_num + n->cols - 1] == 1 )
				k++;
		}

		if( col != n->cols - 1 )
		{
			if( n->vec[ cell_num + n->cols + 1] == 1 )
				k++;
		}
	}

	return k;
}
