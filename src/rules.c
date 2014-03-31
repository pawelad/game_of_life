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
#define char_to_digit(c) (c-'0')

rules_t *file_to_rules( char *filename )
{
	rules_t *r = malloc( sizeof(rules_t) );
	if( r == NULL )
	{
		fprintf( stderr, "Nie można zaalokować pamięci.\n" );
		return NULL;
	}

	// Odczyt z pliku
	FILE *f = fopen( filename, "r" );
	if( f == NULL )
	{
		fprintf( stderr, "Nie można otworzyć pliku.\n" );
		return NULL;
	}

	char line[25];
	fgets( line, 25, f );
	fclose(f);

	r = string_to_rules( line );

	return r;
}

rules_t *string_to_rules( char *string )
{
	rules_t *r = malloc( sizeof(rules_t) );
	if( r == NULL )
	{
		fprintf( stderr, "Nie można zaalokować pamięci.\n" );
		exit( EXIT_FAILURE );
	}

	int i = 0;
	if( !isdigit(string[i]) )
	{
		fprintf( stderr, "Nie można zaalokować pamięci.\n" );
		exit( EXIT_FAILURE );
	}
	while( string[i] != '/' )
	{
		assert( isdigit(string[i]) );
		r->born[i] = char_to_digit( string[i] );
		i++;
	}
	r->born_size = i;
	i++;

	int j = 0;
	assert( isdigit(string[i]) );
	while( string[i] != '\0' )
	{
		assert( isdigit(string[i]) );
		r->lives[j] = char_to_digit( string[i] );
		i++;
		j++;
	}
	r->lives_size = j;

	return r;
}

rules_t *default_rules( )
{
	rules_t *r = malloc( sizeof(rules_t) );
	if( r == NULL )
	{
		fprintf( stderr, "Nie można zaalokować pamięci.\n" );
		exit( EXIT_FAILURE );
	}

	r->born_size = 1;
	r->born[0] = 3;
	r->lives_size = 2;
	r->lives[0] = 2;
	r->lives[1] = 3;

	return r;
}

void rules_to_file( rules_t *rules, char *filename, char *dir )
{
	assert( rules );

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

	for( int i= 0; i < rules->born_size; i++)
		fprintf( f, "%d", rules->born[i] );

	fputc ( '/' , f );

	for( int i= 0; i < rules->lives_size; i++ )
		fprintf( f, "%d", rules->lives[i] );

	fclose( f );
}
