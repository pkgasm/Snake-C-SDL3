#ifndef GAME_H
#define GAME_H

#include "defs.h"

bool init_game(Game* game);
void cleanup_game(Game* game);
void restart_game(Game* game);
void load_high_score(Game *game);
void check_and_save_high_score(Game *game);

#endif // GAME_H