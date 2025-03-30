#include "game.h"
#include "player.h"
#include "bot.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

// Fonction servant à initialiser la variable Game
struct Game* init_game(int target_score) {
    // Réservé dans la mémoire un emplacement qui fait la taille de la structure Game
    struct Game* game = malloc(sizeof(struct Game));
    // Vérifier si le système nous a laissé une place
    if (game == NULL) {
        fprintf(stderr, "Memory allocation error\n");
        exit(EXIT_FAILURE);
    }

    // Met les valeurs null de game
    game->players = NULL;
    game->num_players = 0;
    game->current_player = 0;
    game->target_score = target_score;
    game->game_over = 0;

    return game;
}

// Libérer de l'espace après une partie
void free_game(struct Game* game) {
    if (game != NULL) {
        if (game->players != NULL) {
            // Libérer dans la RAM la liste des joueurs
            free(game->players);
            game->players = NULL;
        }
        // Libérer dans la RAM game
        free(game);
    }
}

// Fonction pour lancer une partie
void play_game(struct Game* game) {
    // Vérifier si la partie n'est pas fini pour joueur un tour
    while (!game->game_over) {
        play_turn(game);
    }

    printf("\n--- FINAL SCORES ---\n");
    display_scores(game);
}

// Fonction pour jouer un tour
void play_turn(struct Game* game) {
    // Récupere le joueur actuel (son tour)
    struct Player* current = &game->players[game->current_player];
    printf("\nPlayer %s, your turn.\n", current->username);

    // Selon son type, effectuer la bonne fonction
    if (current->type != HUMAN) {
        bot_play_turn(current, game);
    } else {
        human_play_turn(current);
    }

    // Vérifier si la personne qui joue à dépasser le nombre de point nécessaire pour gagner
    if (current->score >= game->target_score) {
        printf("\n%s wins with %d points!\n", current->username, current->score);
        game->game_over = 1;
    }

    // Incrémenter de 1 le joueur qui va jouer (soit passer à la personne suivante le tour)
    game->current_player = (game->current_player + 1) % game->num_players;
}

// Montrer un tableau de score
void display_scores(struct Game* game) {
    printf("\nCurrent Scores::\n");
    for (int i = 0; i < game->num_players; i++) {
        printf("%s: %d points\n", game->players[i].username, game->players[i].score);
    }
    printf("\n");
}

// Ajouter dans la liste des joueurs un humain
void add_human_player(struct Game* game, const char* username) {
    // Modifie la taille du pointer dans la RAM
    game->players = realloc(game->players, (game->num_players + 1) * sizeof(struct Player));
    if (game->players == NULL) {
        fprintf(stderr, "Memory allocation error\n");
        exit(EXIT_FAILURE);
    }

    // Met les valeurs null de joueur
    strncpy(game->players[game->num_players].username, username, 49);
    game->players[game->num_players].username[49] = '\0';
    game->players[game->num_players].score = 0;
    game->players[game->num_players].current_turn = 0;
    game->players[game->num_players].type = HUMAN;

    game->num_players++;
}