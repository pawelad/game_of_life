#ifndef MISC_H
#define MISC_H

#define MAX_LINE 1024

#define COLOR_RED "\x1b[31m"
#define COLOR_RESET "\x1b[0m"
#define DEBUG_START(X) printf("\n%sDEBUG: %s%s\n",COLOR_RED,X,COLOR_RESET)
#define DEBUG_END printf("%sDEBUG_END%s\n",COLOR_RED,COLOR_RESET)

// Creates directory if it doesn't exist; if it does it prints error
void create_dir( char *dir );

// Prints error messages
void print_error( char *type );

// Prints file error message
void print_error_file( char *filename );

#endif
