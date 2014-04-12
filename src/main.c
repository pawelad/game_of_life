#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../lib/argparse.h"
#include "gen_sym.h"
#include "netting.h"
#include "rules.h"
#include "to_png.h"
#include "misc.h"

static const char *const usage[] = { "./life_sym [options]", NULL, };

int main( int argc, const char **argv )
{
	#ifdef DEBUG
	DEBUG_START("Arguments");
		if( argc != 0 )
		{
			printf( "argc: %d\n", argc );
			for( int i = 0; i < argc; i++ )
				printf( "argv[%d]: %s\n", i, argv[i] );
		}
	DEBUG_END;
	#endif

	// Wywołane opcje
	char *input       = NULL;
	int gen_num       = 0;
	int photo_num     = 0;
	int scale         = 2;
	char *results_dir = "results";
	char *photo_dir   = "gfx";
	int random        = 0;
	char *mod_file    = NULL;
	char *mod_input   = NULL;

	struct argparse_option options[] = {
		OPT_HELP( ),
		OPT_STRING( 'i', "input", &input, "path to input file", NULL ),
		OPT_INTEGER( 'n', "generation_number", &gen_num, "number of generations to simulate", NULL ),
		OPT_INTEGER( 'p', "photos_number", &photo_num, "number of photos to generate", NULL ),
		OPT_INTEGER( 's', "scale", &scale, "graphic enlargement scale factor (default: 2)", NULL ),
		OPT_STRING( 'd', "dir", &results_dir, "results directory (default: results)", NULL ),
		OPT_STRING( 'D', "photo_dir", &photo_dir, "photos subdirectory (default: gfx)", NULL ),
		OPT_BOOLEAN('r', "random", &random, "creates random net; overrides input file", NULL),
		OPT_STRING( 'm', "mod_file", &mod_file, "path to rules modification file", NULL ),
		OPT_STRING( 'M', "mod_input", &mod_input, "rules modification string", NULL ),
		OPT_END( ),
	};
	struct argparse argparse;
	argparse_init( &argparse, options, usage, 0 );
	argc = argparse_parse( &argparse, argc, argv );


	// Mandatory options
	if( ( !input && random == 0 ) || !gen_num || !photo_num )
	{
		fprintf( stderr, "You didin't run the program with the required options.\n" );
		printf( "\nRequired options:\n");
		printf( "    -i, --input=<str>                 path to input file\n" );
		printf( "    -n, --generation_number=<int>     number of generations to simulate\n" );
		printf( "    -p, --photos_number=<int>         number of photos to generate\n\n" );

		return EXIT_FAILURE;
	}

	if( photo_num > 99999 )
	{
		fprintf( stderr, "%sERROR:%s To big number of photos to generate.\n", COLOR_RED, COLOR_RESET );
		printf("The number of photos to generate must be smaller than 10 000.\n");
		exit(EXIT_FAILURE);
	}


	// Directories
	create_dir( results_dir );
	int photo_path_size = strlen(results_dir) +
						  strlen("/") +
						  strlen(photo_dir) + 1;
	char *photo_path = malloc( photo_path_size );
	if( photo_path == NULL )
		print_error("alloc");
	snprintf( photo_path, photo_path_size, "%s/%s", results_dir, photo_dir );
	create_dir( photo_path );


	// Rules
	rules_t *rules = malloc( sizeof(rules_t) );
	if( rules == NULL )
		print_error("alloc");

	if( mod_file && mod_input )
	{
		fprintf( stderr, "%sERROR:%s You ran the program with both modification file and input.\n", COLOR_RED, COLOR_RESET );
		printf("Run the program with no more than one modification input.\n");
		exit(EXIT_FAILURE);
	}
	else if( mod_file )
	{
		rules = file_to_rules( rules, mod_file );
		rules_to_file( rules, "rules", results_dir );

		#ifdef DEBUG
		DEBUG_START("Rules saved to file");
			printf( "filename: %s/%s\n", results_dir, "rules" );
		DEBUG_END;
		#endif
	}
	else if( mod_input )
	{
		rules = string_to_rules( rules, mod_input );
		rules_to_file( rules, "rules", results_dir );

		#ifdef DEBUG
		DEBUG_START("Rules saved to file");
			printf( "filename: %s/%s\n", results_dir, "rules" );
		DEBUG_END;
		#endif
	}
	else
		rules = default_rules(rules);

	#ifdef DEBUG
	DEBUG_START("Rules");
		printf( "born_size: %d\n", rules->born_size );
		for( int i= 0; i < rules->born_size; i++ )
			printf( "| %d |", rules->born[i] );

		printf("\n");

		printf( "lives_size: %d\n", rules->lives_size );
		for( int i= 0; i < rules->lives_size; i++ )
			printf( "| %d |", rules->lives[i] );
		printf("\n");
	DEBUG_END;
	#endif


	// Net
	net_t *net = malloc( sizeof(net_t) );
	if( net == NULL )
		print_error("alloc");

	if( random == 1 )
		net = random_net( net );
	else
		net = file_to_net( net, input );

	#ifdef DEBUG
	DEBUG_START("Net");
		printf( "rows: %d\ncols: %d\n", net->rows, net->cols );
		if( net->rows <= 20 && net->cols <= 20 )
		{
			for( int i = 0; i < net->rows * net->cols; i++ )
			{
				if( i % net->cols == 0 )
					printf("|");

				if( net->vec[i] == 1 )
					printf( " X |" );
				else
					printf( "   |" );

				if( (i + 1) % net->cols == 0 )
					printf("\n");
			}
		}
	DEBUG_END;
	#endif

	#ifdef DEBUG
	if( net->rows <= 20 && net->cols <= 20 )
	{
	DEBUG_START("Neighborhood");
		for( int i = 0; i < net->rows * net->cols; i++ )
		{
			int nb = cell_neighborhood( net, i );

			if( i % net->cols == 0 )
				printf("|");

			if( nb != 0 )
				printf( " %d |", nb );
			else
				printf("   |");

			if( (i + 1) % net->cols == 0 )
				printf("\n");
		}
	DEBUG_END;
	}
	#endif

	net_to_file( net, "start_net", results_dir );

	#ifdef DEBUG
	DEBUG_START("Net saved to file");
		printf( "filename: %s/start_net\n", results_dir );
	DEBUG_END;
	#endif


	// Tablica od zera; zawsze eksportujemy pierwszą i ostatnią generację
	int leap = (gen_num - 1) / (photo_num - 2);

	#ifdef DEBUG
	DEBUG_START("Generetions numbers to export to photo");
		printf( "gen_num: %d\nphoto_num: %d\nleap: %d\n", gen_num, photo_num, leap );
		for( int i = 0; i < gen_num; i++ )
		{
			if( i == 0 || i % leap == 0 || i == gen_num - 1 )
			{
				if( i % 15 == 0 )
					printf("|");

				if( i < 10 )
					printf( "  %d |", i );
				else
					printf( " %d |", i );

				if( (i + 1) % 15 == 0 )
					printf("\n");
			}
		}
		printf("\n");
	DEBUG_END;
	#endif

	// Photo filename format
	int file_path_size = strlen("/net_") +
						 5 +              // 5 digits numbers
						 strlen(".png") +
						 strlen(photo_path) + 1;
	char *file_path = malloc( file_path_size );
	if( file_path == NULL )
		print_error("alloc");


	// Simulation
	int current_photo = 0;
	for( int i = 0; i < gen_num; i++ )
	{
		if( i == 0 || i % leap == 0 || i == gen_num - 1 )
		{
			snprintf( file_path, file_path_size, "%s/net_%05d.png", photo_path,  current_photo + 1 );
			net_to_png( net, file_path, scale );
			current_photo++;
		}
		net = sym_gen( net, rules );
	}

	// Final net
	net_to_file( net, "end_net", results_dir );

	#ifdef DEBUG
	DEBUG_START("Net saved to file");
		printf( "filename: %s/end_net\n", results_dir );
	DEBUG_END;
	#endif

	printf( "\nAll files are saved in '%s'.\n", results_dir );

	free(photo_path);
	free(file_path);
	free(rules);
	free(net->vec);
	free(net);

	printf( "\nSymulation completed properly.\n" );

	printf("\n");
	return EXIT_SUCCESS;
}
