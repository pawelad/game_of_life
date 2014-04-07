#ifndef RULES_H
#define RULES_H

typedef struct {
	int born_size;   // Number of elements in 'born'
	int born[10];    // Number of neighbors on which dead cells become alive
	int lives_size;  // Number of elements in 'lives'
	int lives[10];   // Number of neighbors on which alive cells stay alive
} rules_t;

// Reading rules from file to struct
rules_t *file_to_rules( rules_t *r, char *filename );

// Saving rules struct from string
rules_t *string_to_rules( rules_t *r, char *string );

// Saving default Game of Life rules struct
rules_t *default_rules( rules_t *r );

// Writing rules struct to file
void rules_to_file( rules_t *rules, char *filename, char *dir );

#endif
