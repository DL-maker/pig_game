#include "game.h"
#include "bot.h"
#include "utils.h"
#include <ctype.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Constantes
#define MAX_USERNAME 50
#define DEFAULT_TARGET_SCORE 100

void setup_game_from_args(struct Game* game, int argc, char** argv);
void setup_game_interactive(struct Game* game);

int main(int argc, char** argv) {
    // Initialisation de la graine pour la fonction rand() en utilisant l'heure actuelle et un ensemble d'autre valeur.
    // Cela permet de générer une séquence de nombres aléatoires différente à chaque exécution du programme.
    unsigned int seed = (unsigned int)time(NULL);
    seed ^= (unsigned int)clock();
    seed ^= (unsigned int)(intptr_t)&seed;
    seed ^= (unsigned int)getpid();
    srand(seed);

    // Initialiser la variable game
    struct Game* game = init_game(DEFAULT_TARGET_SCORE);

    if (argc > 1) {
        setup_game_from_args(game, argc, argv);
    } else {
        setup_game_interactive(game);
    }

    // Vérifie s'il y a assez de joueur
    if (game->num_players < 2) {
        fprintf(stderr, "Error: At least 2 players (humans or bots) are required.\n");
        free_game(game);
        return EXIT_FAILURE;
    }

    play_game(game);

    free_game(game);

    return EXIT_SUCCESS;
}

// Configure le jeu en utilisant les arguments de ligne de commande comme noms de joueurs
void setup_game_from_args(struct Game* game, int argc, char** argv) {
    // Ajoute petit à petit les joueurs humain avec les arguments
    for (int i = 1; i < argc; i++) {
        add_human_player(game, argv[i]);
    }

    // Pose la question de si la personne souhaite ajouter des bots
    char buffer[10];
    char answer;
    printf("Do you want to add bots? (y/n): ");
    if (read_line(buffer, sizeof(buffer)) == NULL) {
        fprintf(stderr, "\nEnd of input detected, existing game\n");
        exit(EXIT_SUCCESS);
    }
    if (strlen(buffer) > 0) {
        answer = tolower(buffer[0]);
        if (answer == 'y') {
            setup_bots(game);
        }
    }

    // Pose la question de si la personne souhaite changer la valeur target
    printf("Target score (default: 100): ");
    char score_input[10];
    if (read_line(score_input, sizeof(score_input)) != NULL && strlen(score_input) > 0) {
        char *endptr;
        long target_score = strtol(score_input, &endptr, 10);
        if (*endptr == '\0' && target_score > 0 && target_score <= INT_MAX) {
            game->target_score = (int)target_score;
        } else {
            printf("Invalid score, using default (100).\n");
        }
    }
}

// Configure le jeu de manière interactive en demandant les informations à l'utilisateur
void setup_game_interactive(struct Game* game) {
    int num_players;
    char username[MAX_USERNAME];
    char buffer[20];

    // Récupere le nombre de joueurs (humain)
    printf("Number of players: ");
    if (read_line(buffer, sizeof(buffer)) != NULL) {
        char* endptr;
        long value = strtol(buffer, &endptr, 10);

        if (*endptr == '\0' && value >= 0 && value <= INT_MAX) {
            num_players = (int)value;
        } else {
            printf("Invalid number, defaulting to 0\n");
            num_players = 0;
        }
    }

    // Demande un nom pour chaque joueur
    for (int i = 0; i < num_players; i++) {
        printf("Player %d name: ", i + 1);
        if (read_line(username, MAX_USERNAME) != NULL && strlen(username) > 0) {
            add_human_player(game, username);
        } else {
            // Si nom vide, générer un nom par défaut
            char default_name[20];
            sprintf(default_name, "Player_%d", i+1);
            add_human_player(game, default_name);
            printf("Using default name: %s\n", default_name);
        }
    }

    // Pose la question de s'il faut ajouter des bots (et si oui, utiliser la fonction setup_bots)
    char answer_buffer[10];
    printf("Do you want to add bots? (y/n): ");
    if (read_line(answer_buffer, sizeof(answer_buffer)) != NULL && strlen(answer_buffer) > 0) {
        char answer = tolower(answer_buffer[0]);
        if (answer == 'y') {
            setup_bots(game);
        }
    }

    // Pose la question de s'il faut changer le target score
    printf("Target score (default: 100): ");
    char score_input[10];
    if (read_line(score_input, sizeof(score_input)) != NULL && strlen(score_input) > 0) {
        char* endptr;
        long target_score = strtol(score_input, &endptr, 10);
        if (*endptr == '\0' && target_score > 0 && target_score <= INT_MAX) {
            game->target_score = (int)target_score;
        } else {
            printf("Invalid score, using default (100).\n");
        }
    }
}