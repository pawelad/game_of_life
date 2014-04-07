#ifndef MISC_H
#define MISC_H

#define COLOR_RED "\x1b[31m"
#define COLOR_RESET "\x1b[0m"
#define DEBUG_START(X) printf("\n%sDEBUG: %s %s\n",COLOR_RED,X,COLOR_RESET)
#define DEBUG_END printf("%sDEBUG_END %s\n",COLOR_RED,COLOR_RESET)

// Creates directory if it doesn't exist; if it does it prints error
void create_dir( char *dir );

#endif
