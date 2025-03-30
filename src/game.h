#ifndef GAME_H
#define GAME_H

#include <stdio.h>
#include <stdlib.h>

// Enum servant à simplifier au développeur
enum PlayerType {
    HUMAN,
    BOT_EASY,
    BOT_MEDIUM,
    BOT_HARD,
    BOT_RANDOM
};

struct Player {
    char username[50]; // Son nom
    int score; // Son score dans sa banque
    int current_turn; // Le score qu'il a dans un tour
    enum PlayerType type; // Le type de joueur, soit humain soit un bot (avec difficulté)
};

struct Game {
    struct Player* players; // Liste des joueurs
    int num_players; // Nombre de joueur
    int current_player; // Le tour du joueur (position dans la liste)
    int target_score; // Le score à atteindre pour gagner la partie
    int game_over; // Booléan pour savoir si c'est la fin de la partie
};

struct Game* init_game(int target_score);
void free_game(struct Game* game);
void play_game(struct Game* game);
void play_turn(struct Game* game);
void display_scores(struct Game* game);
void add_human_player(struct Game* game, const char* username);

#endif