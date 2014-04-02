#ifndef RULES_H
#define RULES_H

// Struktura przechowująca zasady
typedef struct {
	int born_size;   // Ilośc elementów w born
	int born[10];    // Cyfry dla których martwe komórki ożywają
	int lives_size;  // Ilość elementów w lives
	int lives[10];   // Cyfry dla których żywe komórki przeżywają 
} rules_t;

// Tworzy zasady z pliku
rules_t *file_to_rules( rules_t *r, char *filename );

// Tworzy zasady z ciągu znaków
rules_t *string_to_rules( rules_t *r, char *string );

// Tworzy domyślne zasady
rules_t *default_rules( rules_t *r );

// Eksportuje zasady do pliku
void rules_to_file( rules_t *rules, char *filename, char *dir );

#endif
