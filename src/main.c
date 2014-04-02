#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "../lib/argparse.h"
#include "gen_sym.h"
#include "netting.h"
#include "rules.h"
#include "png.h"
#include "misc.h"

static const char *const usage[] = { "life_sym [options]", NULL, };

int main( int argc, const char **argv )
{
	// Wywołane opcje
	char *input       = NULL;
	char *output      = NULL;
	int gen_num       = 0;
	int photo_num     = 0;
	char *results_dir = NULL;
	char *photo_dir   = NULL;
	char *mod_file    = NULL;
	char *mod_input   = NULL;

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

	#ifdef DEBUG
	DEBUG_START("Wprowadzone argumenty");
		if( argc != 0 )
		{
			printf( "argc: %d\n", argc );
			for( int i= 0; i < argc; i++ )
				printf( "argv[%d]: %s\n", i, *(argv + i) );
		}
	DEBUG_END;
	#endif


	// Sprawdzanie czy program został poprawnie wywołany
	if( !input || !output || !gen_num || !photo_num )
	{
		fprintf( stderr, "Nie wywołałeś programu z wymaganymi opcjami.\n" );
		printf( "\nWymagane opcje to:\n");
		printf( "    -i, --input=<str>                 path to input file\n" );
		printf( "    -o, --output=<str>                path to output file\n" );
		printf( "    -n, --generation_number=<int>     number of generations to simulate\n" );
		printf( "    -p, --photos_number=<int>         number of photos to generate\n\n" );

		return EXIT_FAILURE;
	}


	// Domyślne nazwy folderów
	if( !results_dir )
	{
		results_dir = malloc( 8 * sizeof(char) );
		if( results_dir == NULL )
		{
			fprintf( stderr, "Nie można zaalokować pamięci.\n" );
			exit( EXIT_FAILURE );
		}
		strcpy( results_dir, "results" );
	}

	if( !photo_dir )
	{
		photo_dir = malloc( 4 * sizeof(char) );
		if( results_dir == NULL )
		{
			fprintf( stderr, "Nie można zaalokować pamięci.\n" );
			exit( EXIT_FAILURE );
		}
		strcpy( photo_dir, "gfx" ); 
	}


	// Zasady
	rules_t *rules = malloc( sizeof(rules_t) );
	if( rules == NULL )
	{
		fprintf( stderr, "Nie można zaalokować pamięci.\n" );
		exit(EXIT_FAILURE);
	}

	if( mod_file && mod_input )
	{
		fprintf( stderr, "Wywołałeś program jednocześnie ze ścieżką do pliku z zasadami oraz z zasadami.\n" );
		printf( "\nWywołaj program z nie więcej niż jedną modyfikacją zasad.\n\n" );
		return EXIT_FAILURE;
	}
	else if( mod_file )
	{
		rules = file_to_rules( rules, mod_file );
		rules_to_file( rules, "rules", results_dir );

		#ifdef DEBUG
		DEBUG_START( "Zasady zapisane do pliku" );
			printf("filename: %s/%s\n", results_dir, "rules" );
		DEBUG_END;
		#endif
	}
	else if( mod_input )
	{
		rules = string_to_rules( rules, mod_input );
		rules_to_file( rules, "rules", results_dir );

		#ifdef DEBUG
		DEBUG_START( "Zasady zapisane do pliku" );
			printf("filename: %s/%s\n", results_dir, "rules" );
		DEBUG_END;
		#endif
	}
	else
		rules = default_rules( rules );

	#ifdef DEBUG
	DEBUG_START( "Używane zasady" );
		printf( "born_size: %d\n", rules->born_size );
		for( int i= 0; i < rules->born_size; i++ )
			printf( "| %d |", rules->born[i] );
		printf( "\n" );
		printf( "lives_size: %d\n", rules->lives_size );
		for( int i= 0; i < rules->lives_size; i++ )
			printf( "| %d |", rules->lives[i] );
		printf( "\n" );
	DEBUG_END;
	#endif


	// Siatka
	net_t *net = malloc( sizeof(net_t) );
	if( net == NULL )
	{
		fprintf( stderr, "Nie można zaalokować pamięci.\n" );
		exit(EXIT_FAILURE);
	}

	net = file_to_net( net, input );

	#ifdef DEBUG
	DEBUG_START( "Wprowadzona siatka" );
		printf( "rows: %d\n", net->rows );
		printf( "cols: %d\n", net->cols );
		printf( "vec_size: %d\n", net->vec_size );
		for( int i= 0; i < net->vec_size; i++ )
			printf( "| %d |", net->vec[i] );
		printf( "\n" );
	DEBUG_END;
	#endif

	char *basename = filename_from_path( input );
	net_to_file( net, basename, results_dir );

	#ifdef DEBUG
	DEBUG_START( "Siatka zapisana do pliku" );
		printf("filename: %s/%s\n", results_dir, basename );
	DEBUG_END;
	#endif


	// Tablica od zera; zawsze eksportujemy pierwszą i ostatnią generację
	int leap = (gen_num - 1) / (photo_num - 2);
	int current_photo = 0;

	#ifdef DEBUG
	DEBUG_START( "Indeksy generacji do wyeksportowania" );
		printf( "gen_num: %d\nphoto_num: %d\nleap: %d\n", gen_num, photo_num, leap );
	#endif

	for( int i= 0; i < gen_num; i++ )
	{
		if( i == 0 || i % leap == 0 || i == gen_num - 1 )
		{
			net_to_png( net, current_photo, results_dir, photo_dir );
			current_photo++;

			#ifdef DEBUG
				printf( "| %d |", i );
			#endif
		}
		net = sym_gen( net, rules );
	}

	#ifdef DEBUG
		printf( "\n" );
	DEBUG_END;
	#endif


	// Zapis wyników
	net_to_file( net, output, results_dir );

	#ifdef DEBUG
	DEBUG_START( "Siatka zapisana do pliku" );
		printf("filename: %s/%s\n", results_dir, output );
	DEBUG_END;
	#endif


	free(results_dir);
	free(photo_dir);
	free(rules);
	free(net->vec);
	free(net);


	printf( "\nSymulacja przeprowadzona poprawnie.\n" );

	printf( "\n" );
	return EXIT_SUCCESS;
}
