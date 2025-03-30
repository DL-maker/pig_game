#include "player.h"
#include "utils.h"
#include <stdio.h>
#include <ctype.h>
#include <time.h>

// Gère le tour d'un joueur humain, lui permettant de lancer le dé et de décider quand banquer
void human_play_turn(struct Player* player) {
    // Initialise les variables
    int dice_value;
    char choice;

    player->current_turn = 0;

    while (1) {
        // Met une valeur random dans dice_value (1-6)
        dice_value = roll_dice();
        printf("    You rolled a %d\n", dice_value);

        // Perd son tour
        if (dice_value == 1) {
            printf("    No luck :( :(\n");
            printf("    You have %d point in bank.\n", player->score);
            player->current_turn = 0;
            break;
        }

        player->current_turn += dice_value;

        printf("    You have %d point in bank, and %d points for this turn.\n", player->score, player->current_turn);

        // Récupere le choix de l'utilisateur ('r' or 'b')
        choice = get_player_choice();

        // Si le joueur choisit 'b', bank les points
        if (choice == 'b') {
            player->score += player->current_turn;
            printf("    You have %d points in bank.\n", player->score);
            break;
        }
    }
}

// Demande et valide le choix du joueur (continuer à lancer (r) ou banquer les points (b))
char get_player_choice() {
    char choice;
    char buffer[10]; // Buffer temporaire

    while (1) {
        printf("    Continue [r]olling, or [b]ank ? ");
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            fprintf(stderr, "\nEnd of input detected, exising game\n");
            exit(EXIT_SUCCESS);
        }
        
        // Prendre uniquement le premier caractère
        choice = tolower(buffer[0]);
    
        // Vérifie si l'input est valide
        if (choice == 'r' || choice == 'b') {
            return choice;
        }
        printf("    Invalid option. Please enter 'r' to roll again or 'b' to bank.\n");
    }
}