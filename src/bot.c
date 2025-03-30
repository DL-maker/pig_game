#include "bot.h"
#include "utils.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

// Stratégie de difficulté facile: continue à lancer si moins de 5 points, sinon décision aléatoire
int bot_easy_strategy(struct Player* bot) {    
    if (bot->current_turn < 5) {
        return 1; // Continue à lancer si moins de 5 points
    }

    return (rand() % 2); // Décision aléatoire après 5 points: 0 = banque, 1 = continue
}

// Stratégie de difficulté moyen: banque si 10+ points ou si peut gagner, sinon continue
int bot_medium_strategy(struct Player* bot, struct Game* game) {
    if (bot->current_turn >= 10) {
        return 0; // Banque si 10 points ou plus dans ce tour
    }

    if (bot->score + bot->current_turn >= game->target_score) {
        return 0; // Banque s'il peut gagner avec le score actuel
    }

    return 1; // Continue à lancer dans les autres cas
}

// Stratégie de difficulté difficile: prend en compte le score des adversaires et l'état du jeu
int bot_hard_strategy(struct Player* bot, struct Game* game) {
    // Trouve le score maximum parmi les adversaires
    int max_opponent_score = 0;
    for (int i = 0; i < game->num_players; i++) {
        if (&game->players[i] != bot && game->players[i].score > max_opponent_score) {
            max_opponent_score = game->players[i].score;
        }
    }

    // Banque si peut gagner avec le score actuel
    if (bot->score + bot->current_turn >= game->target_score) {
        return 0;
    }

    // Si un adversaire a une avance significative, prend plus de risques
    if (max_opponent_score > bot->score + 20) {
        return (bot->current_turn < 15); // Continue jusqu'à 15 points
    }

    // Si un adversaire est proche de gagner, prend plus de risques
    if (max_opponent_score >= game->target_score - 20) {
        return (bot->current_turn < 15); // Ccontinue jusqu'à 15 points
    }

    // Si proche de gagner, joue plus prudemment
    if (bot->score >= game->target_score - 30) {
        return (bot->current_turn < 8); // Continue jusqu'à 8 points seulement
    }

    // Comportement par défaut
    return (bot->current_turn < 12); // Continue jusqu'à 12 points
}

// Stratégie aléatoire: banque si peut gagner, sinon décision aléatoire
int bot_random_strategy(struct Player* bot, struct Game* game) {
    if (bot->score + bot->current_turn >= game->target_score) {
        return 0; // Banque si peut gagner
    }

    return (rand() % 2); // Décision aléatoire dans les autres cas
}

// Détermine si le bot doit continuer à lancer le dé ou banquer ses points selon sa difficulté
int bot_decision(struct Player* bot, struct Game* game) {
    switch (bot->type) {
        case BOT_EASY:
            return bot_easy_strategy(bot);
        case BOT_MEDIUM:
            return bot_medium_strategy(bot, game);
        case BOT_HARD:
            return bot_hard_strategy(bot, game);
        case BOT_RANDOM:
            return bot_random_strategy(bot, game);
        default:
            return 0; // Par défaut, banque les points
    }
}

// Gère le tour d'un bot, simule ses lancers et décisions selon sa stratégie
void bot_play_turn(struct Player* bot, struct Game* game) {
    int dice_value;
    bot->current_turn = 0;

    printf("Bot %s is playing...\n", bot->username);

    while (1) {
        // Lance le dé
        dice_value = roll_dice();
        printf("    %s rolled a %d\n", bot->username, dice_value);

        // Si 1, perd tous les points du tour
        if (dice_value == 1) {
            printf("    Bad luck! %s loses all points for this turn.\n", bot->username);
            bot->current_turn = 0;
            break;
        }

        // Ajoute la valeur de dé au score du tour
        bot->current_turn += dice_value;
        printf("    %s has %d points in bank, and %d points for this turn.\n", bot->username, bot->score, bot->current_turn);

        // Consulte la stratégie du bot pour décider de continuer ou non
        if (!bot_decision(bot, game)) {
            printf("    %s decides to bank points.\n", bot->username);
            bot->score += bot->current_turn;
            printf("    %s now has %d points in bank.\n", bot->username, bot->score);
            break;
        }

        printf("    %s decides to roll again.\n", bot->username);
        
        sleep(1);
    }
}

// Initialise un bot avec la difficulté et le numéro spécifiés
void init_bot(struct Player* bot, enum PlayerType difficulty, int bot_num) {
    char difficulty_name[10];

    // Détermine le nom de la difficulté
    switch (difficulty) {
        case BOT_EASY:
            strcpy(difficulty_name, "Easy");
            break;
        case BOT_MEDIUM:
            strcpy(difficulty_name, "Medium");
            break;
        case BOT_HARD:
            strcpy(difficulty_name, "Hard");
            break;
        case BOT_RANDOM:
            strcpy(difficulty_name, "Random");
            break;
        default:
            strcpy(difficulty_name, "Unknown");
    }

    // Crée le nom du bot avec sa difficulté et son numéro
    snprintf(bot->username, sizeof(bot->username), "Bot-%s-%d", difficulty_name, bot_num);
    bot->score = 0;
    bot->current_turn = 0;
    bot->type = difficulty;
}

// Ajoute un bot au jeu avec la difficulté spécifiée
void add_bot(struct Game* game, enum PlayerType difficulty) {
    // Réalloue la mémoire pour accueillir un joueur supplémentaire
    game->players = realloc(game->players, (game->num_players + 1) * sizeof(struct Player));
    if (game->players == NULL) {
        fprintf(stderr, "Memory allocation error.\n");
        exit(EXIT_FAILURE);
    }

    // Initialise le bot et l'ajoute à la liste des joueurs
    init_bot(&game->players[game->num_players], difficulty, game->num_players);
    game->num_players++;
}

// Configure les bots de manière interactive en demandant leur nombre et difficulté
void setup_bots(struct Game* game) {
    int num_bots = 0;
    int difficulty = 0;
    char buffer[20];

    // Demande le nombre de bots à ajouter
    printf("How many bots would you like to add? ");
    if (read_line(buffer, sizeof(buffer)) != NULL) {
        char* endptr;
        long value = strtol(buffer, &endptr, 10);

        if (*endptr == '\0' && value >= 0 && value <= INT_MAX) {
            num_bots = (int)value;
        } else {
            printf("Invalid numver, defaulting to 0\n");
            num_bots = 0;
        }
    }

    // Pour chaque bot, demande sa difficulté
    for (int i = 0; i < num_bots; i++) {
        printf("Select difficulty for Bot %d:\n", i + 1);
        printf("1. Easy\n");
        printf("2. Medium\n");
        printf("3. Hard\n");
        printf("4. Random\n");
        printf("5. Random Selection (choose a random difficulty)\n");
        printf("Choice: ");

        if (read_line(buffer, sizeof(buffer)) != NULL) {
            char *endptr;
            long value = strtol(buffer, &endptr, 10);

            if (*endptr == '\0' && value >= 1 && value <= 5) {
                difficulty = (int)value;
            } else {
                printf("Invalid choice, defaulting to Easy\n");
                difficulty = 1;
            }
        } else {
            printf("Invalid input, defaulting to Easy\n");
            difficulty = 1;
        }

        // Si option 5 choisie, sélectionne une difficulté aléatoire
        if (difficulty == 5) {
            difficulty = (rand() % 4) + 1;
            printf("Randomly selected difficulty: ");
            switch (difficulty) {
                case 1: printf("Easy\n"); break;
                case 2: printf("Medium\n"); break;
                case 3: printf("Hard\n"); break;
                case 4: printf("Random\n"); break;
            }
        }

        // Ajoute le bot avec la difficulté choisie ou aléatoire
        switch (difficulty) {
            case 1:
                add_bot(game, BOT_EASY);
                break;
            case 2:
                add_bot(game, BOT_MEDIUM);
                break;
            case 3:
                add_bot(game, BOT_HARD);
                break;
            case 4:
                add_bot(game, BOT_RANDOM);
                break;
            default:
                add_bot(game, BOT_EASY); // Par défaut, difficulté facile
                break;
        }
    }
}