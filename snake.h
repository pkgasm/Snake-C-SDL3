#ifndef SNAKE_H
#define SNAKE_H

#include "defs.h"

void init_snake(Snake* snake);
void generate_food(Game* game);
void update_snake(Game* game);

#endif // SNAKE_H