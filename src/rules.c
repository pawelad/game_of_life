#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "rules.h"

// Zamiana char na int
#define to_digit(c) (c-'0')

rules_t *file_to_rules( char *filename )
{
	rules_t *r = malloc( sizeof(rules_t) );
	assert( r != NULL );

	FILE *f = fopen( filename, "r" );
	assert( f != NULL);

	char line[25];
	fgets( line, 25, f );

	r = string_to_rules( line );

	fclose(f);

	return r;
}

rules_t *string_to_rules( char *string )
{
	rules_t *r = malloc( sizeof(rules_t) );
	assert( r != NULL );

	int i = 0;
	assert( isdigit(string[i]) );
	while( string[i] != '/' )
	{
		assert( isdigit(string[i]) );
		r->born[i] = to_digit( string[i] );
		i++;
	}
	r->born_size = i;
	i++;

	int j = 0;
	assert( isdigit(string[i]) );
	while( string[i] != '\0' )
	{
		assert( isdigit(string[i]) );
		r->lives[j] = to_digit( string[i] );
		i++;
		j++;
	}
	r->lives_size = j;

	return r;
}

rules_t *default_rules()
{
	rules_t *r = malloc( sizeof(rules_t) );
	assert( r != NULL );

	r->born_size = 1;
	r->born[0] = 3;
	r->lives_size = 2;
	r->lives[0] = 2;
	r->lives[1] = 3;

	return r;
}

void rules_to_file( rules_t *rules, char *filename, char *dir )
{
	struct stat st = {0};
	if ( stat( dir, &st ) == -1 )
		mkdir( dir, 0700 );

	char path[strlen(filename) + strlen(dir) + 2];
	strcpy( path, dir );
	strcat( path, "/" );
	strcat( path, filename );

	FILE *f = fopen( path, "w+" );
	assert( f != NULL);

	for( int i= 0; i < rules->born_size; i++)
		fprintf(f, "%d", rules->born[i]);

	fputc ( '/' , f );

	for( int i= 0; i < rules->lives_size; i++)
		fprintf(f, "%d", rules->lives[i]);

	fclose(f);
}
