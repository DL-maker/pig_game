#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Fonction qui renvoie une valeur entre 1 et 6
int roll_dice() {
    return (rand() % 6) + 1;
}

// Vider le buffer d'entrée pour éviter les problèmes de lecture successive (par exemple compter le bouton entrée)
void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Lit une ligne depuis l'entrée et la stocke dans le buffer founi
// Retourne le pointeur vers le buffer en cas de succès
char* read_line(char* buffer, int size) {
    if (fgets(buffer, size, stdin) != NULL) {
        size_t len = strlen(buffer);
        // Supprime le caractère de nouvelle ligne s'il est présent
        if (len > 0 && buffer[len-1] == '\n') {
            buffer[len-1] = '\0';
        } else {
            // Si la ligne est plus longue que le buffer, tronquer la chaîne
            buffer[size-1] = '\0';
            // Vide le reste du buffer d'entrée
            clear_input_buffer();
            fprintf(stderr, "Warning: Input truncated to %d characters\n", size-1);
        }
        return buffer;
    }
    return NULL;
}