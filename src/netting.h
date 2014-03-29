#ifndef NETTING_H
#define NETTING_H

// Struktura przechowująca siatkę
typedef struct {
	int rows;
	int cols;
	int *vec;
} net_t;

// Odczyt siatki z pliku
net_t file_to_net( char *filename );

// Zapis siatki do pliku
void net_to_file( net_t net, char *filename, char *dir );

#endif
