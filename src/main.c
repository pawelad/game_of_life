#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../lib/argparse.h"
#include "gen_sym.h"
#include "netting.h"
#include "png.h"
#include "rules.h"

#define COLOR_RED "\x1b[31m"
#define COLOR_RESET "\x1b[0m"
#define DEBUG_START(X) printf("\n%sDEBUG: %s %s\n",COLOR_RED,X,COLOR_RESET)
#define DEBUG_END printf("%sDEBUG %s\n",COLOR_RED,COLOR_RESET)

// Deklaracja globalnych zmiennych
char *input       = NULL;
char *output      = NULL;
int gen_num       = 0;
int photo_num     = 0;
char *results_dir = NULL;
char *photo_dir   = NULL;
char *mod_file    = NULL;
char *mod_input   = NULL;

static const char *const usage[] = { "life_sym [options]", NULL,};

void parse_arg( int argc, const char **argv );

int main( int argc, const char **argv )
{
	
	parse_arg( argc, argv );

	#ifdef DEBUG
	DEBUG_START("Wprowadzone opcje i argumenty");
		if( argc != 0 )
		{
			printf("argc: %d\n", argc);
			int i;
			for (i = 0; i < argc; i++) {
				printf("argv[%d]: %s\n", i, *(argv + i));
			}
		}
	DEBUG_END;
	#endif

	// Sprawdzanie czy program został poprawnie wywołany
	if( !input || !output || !gen_num || !photo_num )
	{
		fprintf(stderr, "Nie wywołałeś programu z wymaganymi opcjami.\n");
		printf("\nWymagane opcje to:\n");
		printf("    -i, --input=<str>                 path to input file\n");
		printf("    -o, --output=<str>                path to output file\n");
		printf("    -n, --generation_number=<int>     number of generations to simulate\n");
		printf("    -p, --photos_number=<int>         number of photos to generate\n\n");

		return -1;
	}

	net_t net = file_to_net( input );
	net_to_file( net, input, results_dir );
	
	// Domyślne wartości
	if( !results_dir ){
		results_dir = malloc( 8 * sizeof(&results_dir) );
		if( results_dir == NULL )
		{
			fprintf(stderr, "Nie udało się zaalokować pamięci.\n");
			return -2;
		}
		strcpy(results_dir, "results");
	}

	if( !photo_dir ){
		photo_dir = malloc( 4 * sizeof(&results_dir) );
		if( photo_dir == NULL )
		{
			fprintf(stderr, "Nie udało się zaalokować pamięci.\n");
			return -2;
		}
		strcpy(photo_dir, "gfx"); 
	}


	rules_t rules;
	if( !mod_file && !mod_input )
	{
		fprintf(stderr, "Wywołałeś program jednocześnie ze ścieżką do pliku z zasadami oraz z zasadami.\n");
		printf("\nWywołaj program z nie więcej niż jedną modyfikacją zasad.\n\n");
		return -1;
	}
	else if( !mod_file )
	{
		rules = file_to_rules( mod_file );
		rules_to_file( rules, "rules", results_dir );
	}
	else if( !mod_input )
	{
		rules = string_to_rules( mod_input );
		rules_to_file( rules, "rules", results_dir );
	}
	else
	{
		rules = default_rules();
	}


	// Tablica od zera; zawsze eksportujemy pierwszą i ostatnią generację
	int leap = (gen_num - 1) / (photo_num - 2);
	int current_photo = 0;

	#ifdef DEBUG
	DEBUG_START("Indeksy generacji do wyeksportowania");
	printf("gen_num: %d\nphoto_num: %d\nleap: %d\n", gen_num, photo_num, leap );
	#endif

	for( int i= 0; i < gen_num; i++ )
	{
		if( i == 0 || i % leap == 0 || i == gen_num - 1 )
		{
			net_to_png( net, current_photo, results_dir, photo_dir );
			current_photo++;
			#ifdef DEBUG
				printf("| %d |", i);
			#endif
		}
		net = sym_gen( net, rules );
	}

	#ifdef DEBUG
	printf("\n");
	DEBUG_END;
	#endif

	net_to_file( net, output, results_dir );
	printf("\nSymulacja przeprowadzona poprawnie.\n");


	printf("\n");
	return EXIT_SUCCESS;
}

void parse_arg( int argc, const char **argv )
{
	struct argparse_option options[] = {
		OPT_HELP(),
		OPT_STRING('i', "input", &input, "path to input file", NULL),
		OPT_STRING('o', "output", &output, "path to output file", NULL),
		OPT_INTEGER('n', "generation_number", &gen_num, "number of generations to simulate", NULL),
		OPT_INTEGER('p', "photos_number", &photo_num, "number of photos to generate", NULL),
		OPT_STRING('d', "dir", &results_dir, "directory for results", NULL),
		OPT_STRING('D', "photo_dir", &photo_dir, "subdirectory for photos", NULL),
		OPT_STRING('m', "mod_file", &mod_file, "path to rules modification file", NULL),
		OPT_STRING('M', "mod_input", &mod_input, "rules modification", NULL),
		OPT_END(),
	};
	struct argparse argparse;
	argparse_init(&argparse, options, usage, 0);
	argc = argparse_parse(&argparse, argc, argv);
}
