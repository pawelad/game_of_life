#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>

#include "rules.h"
#include "misc.h"

// Zamiana char na int
#define char_to_digit(c) (c-'0')

rules_t *file_to_rules( rules_t *r, char *filename )
{
	assert( r != NULL );

	// Odczyt z pliku
	FILE *f = fopen( filename, "r" );
	if( f == NULL )
	{
		fprintf( stderr, "Nie można otworzyć pliku: %s\n", filename );
		exit(EXIT_FAILURE);
	}

	fseek( f, 0, SEEK_END );
	long int length = ftell( f );
	fseek( f, 0, SEEK_SET );
	char *buffer = malloc( ( length + 1 ) * sizeof(char) );
	if( buffer == NULL )
	{
		fprintf( stderr, "Nie można zaalokować pamięci.\n" );
		exit(EXIT_FAILURE);
	}
	memset( buffer, '\0', length + 1 );
	fread( buffer, 1, length, f );

	r = string_to_rules( r, buffer );
	
	free(buffer);

	fclose(f);

	return r;
}

rules_t *string_to_rules( rules_t *r, char *string )
{
	assert( string != NULL );

	if( strlen(string) > 21 )
	{
		fprintf( stderr, "Plik z zasadami ma za dużo znaków.\n" );
		exit(EXIT_FAILURE);
	}

	int i = 0;
	while( string[i] != '/' )
	{
		printf("%c\n", string[i]);
		if( !isdigit(string[i]) || i > 9 )
		{
			fprintf( stderr, "Nie poprawny format pliku z zasadami.\n" );
			exit(EXIT_FAILURE);
		}

		r->born[i] = char_to_digit(string[i]);
		i++;
	}
	r->born_size = i;
	i++;

	int j = 0;
	while( string[i] != '\0' )
	{
		printf("%c\n", string[i]);
		if( !isdigit(string[i]) || j > 9 )
		{
			fprintf( stderr, "Nie poprawny format pliku z zasadami.\n" );
			exit(EXIT_FAILURE);
		}

		r->lives[j] = char_to_digit(string[i]);
		i++;
		j++;
	}
	r->lives_size = j;

	return r;
}

rules_t *default_rules( rules_t *r )
{
	r->born_size = 1;
	r->born[0] = 3;
	r->lives_size = 2;
	r->lives[0] = 2;
	r->lives[1] = 3;

	return r;
}

void rules_to_file( rules_t *rules, char *filename, char *dir )
{
	assert( rules != NULL );

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

	for( int i= 0; i < rules->born_size; i++ )
		fprintf( f, "%d", rules->born[i] );

	fputc ( '/', f );

	for( int i= 0; i < rules->lives_size; i++ )
		fprintf( f, "%d", rules->lives[i] );

	fclose(f);
}
