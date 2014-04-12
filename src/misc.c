#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "misc.h"

void create_dir( char *dir )
{
	struct stat st = { 0 };
	if( stat( dir, &st ) == -1 )
	{
		mkdir( dir, 0700 );
	}
}

void print_error( char *type )
{
	if( strcmp( type, "alloc" ) == 0 )
	{
		fprintf( stderr, "%sERROR:%s Can't allocate memory.\n", COLOR_RED, COLOR_RESET );
	}
	else if( strcmp( type, "rules" ) == 0 )
	{
		fprintf( stderr, "%sERROR:%s Incorrect rules file.\n", COLOR_RED, COLOR_RESET );
	}
	else if( strcmp( type, "net" ) == 0 )
	{
		fprintf( stderr, "%sERROR:%s Incorrect net file.\n", COLOR_RED, COLOR_RESET );
	}
	else
	{
		fprintf( stderr, "%sERROR%s\n", COLOR_RED, COLOR_RESET );
	}

	exit(EXIT_FAILURE);
}

void print_error_file( char *filename )
{
	fprintf( stderr, "%sERROR:%s Can't open file: %s\n", COLOR_RED, COLOR_RESET, filename );
	exit(EXIT_FAILURE);
}
