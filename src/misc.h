#ifndef MISC_H
#define MISC_H

#define COLOR_RED "\x1b[31m"
#define COLOR_RESET "\x1b[0m"
#define DEBUG_START(X) printf("\n%sDEBUG: %s %s\n",COLOR_RED,X,COLOR_RESET)
#define DEBUG_END printf("%sDEBUG_END %s\n",COLOR_RED,COLOR_RESET)

// Stworzenie katalogu jeżeli nie istnieje
void make_dir( char *dir );

// Stworzenie ścieżki do pliku
char *create_path( char *filename, char *dir );

// Nazwa pliku ze ścieżki
char *filename_from_path( char *path );

#endif
