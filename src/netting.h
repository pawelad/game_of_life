#ifndef NETTING_H
#define NETTING_H

// Netting struct
typedef struct {
	int rows;
	int cols;
	unsigned char *vec; // Contains 0 and 1's
} net_t;

// Reading net from file to struct
net_t *file_to_net( net_t *n, char *filename );

// Generates random net
net_t *random_net( net_t *n );

// Writing net struct to file
void net_to_file( net_t *n, char *filename, char *dir );

#endif
