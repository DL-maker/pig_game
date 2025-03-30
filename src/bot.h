#ifndef BOT_H
#define BOT_H

#include "game.h"

void bot_play_turn(struct Player* bot, struct Game* game);
int bot_decision(struct Player* bot, struct Game* game);
int bot_easy_strategy(struct Player* bot);
int bot_medium_strategy(struct Player* bot, struct Game* game);
int bot_hard_strategy(struct Player* bot, struct Game* game);
int bot_random_strategy(struct Player* bot, struct Game* game);
void init_bot(struct Player* bot, enum PlayerType difficulty, int bot_num);
void add_bot(struct Game* game, enum PlayerType difficulty);
void setup_bots(struct Game* game);

#endif