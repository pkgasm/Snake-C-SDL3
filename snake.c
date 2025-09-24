#include "snake.h"
#include "game.h" // Para check_and_save_high_score
#include <stdlib.h>

static bool check_collision(Snake* snake);
static bool check_food_collision(Snake* snake, Food* food);

void init_snake(Snake* snake) {
    snake->length = 3;
    snake->direction = DIR_RIGHT;
    snake->next_direction = DIR_RIGHT;
    
    // Posición inicial en el centro
    int start_x = GRID_WIDTH / 2;
    int start_y = GRID_HEIGHT / 2;
    
    for (int i = 0; i < snake->length; i++) {
        snake->segments[i].x = start_x - i;
        snake->segments[i].y = start_y;
    }
}

void generate_food(Game* game) {
    bool valid_position = false;
    
    // Decidir aleatoriamente el tipo de comida
    int chance = rand() % 100; // 0-99
    if (chance < 80) { // 80% de probabilidad de comida normal
        game->current_food.type = FOOD_NORMAL;
    } else if (chance < 90) { // 10% de score boost
        game->current_food.type = FOOD_SCORE_BOOST;
    } else { // 10% de speed down
        game->current_food.type = FOOD_SPEED_DOWN;
    }

    while (!valid_position) {
        game->current_food.pos.x = rand() % GRID_WIDTH;
        game->current_food.pos.y = rand() % GRID_HEIGHT;
        
        valid_position = true;
        // Verificar que la comida no aparezca sobre la serpiente
        for (int i = 0; i < game->snake.length; i++) {
            if (game->snake.segments[i].x == game->current_food.pos.x && 
                game->snake.segments[i].y == game->current_food.pos.y) {
                valid_position = false;
                break;
            }
        }
    }
}

void update_snake(Game* game) {
    Snake* snake = &game->snake;
    
    snake->direction = snake->next_direction;
    
    Position new_head = snake->segments[0];
    
    switch (snake->direction) {
        case DIR_UP:    new_head.y--; break;
        case DIR_DOWN:  new_head.y++; break;
        case DIR_LEFT:  new_head.x--; break;
        case DIR_RIGHT: new_head.x++; break;
    }
    
    if (new_head.x < 0 || new_head.x >= GRID_WIDTH || 
        new_head.y < 0 || new_head.y >= GRID_HEIGHT) {
        game->state = STATE_GAME_OVER;
        check_and_save_high_score(game);
        return;
    }
    
    for (int i = snake->length - 1; i > 0; i--) {
        snake->segments[i] = snake->segments[i - 1];
    }
    
    snake->segments[0] = new_head;
    
    if (check_food_collision(snake, &game->current_food)) {
        snake->length++;

        switch (game->current_food.type) {
            case FOOD_NORMAL:
                game->score += 10;
                break;
            case FOOD_SCORE_BOOST:
                game->score += 50;
                break;
            case FOOD_SPEED_DOWN:
                game->score += 10;
                // Ralentiza durante 5 segundos
                game->speed = INITIAL_SPEED + 50;
                game->powerup_end_time = SDL_GetTicks() + 5000;
                break;
        }

        if (snake->length == MAX_SNAKE_LENGTH) {
            game->state = STATE_GAME_OVER;
            game->game_won = true;
            check_and_save_high_score(game);
        } else {
            generate_food(game);
        }
        
        if (game->current_food.type == FOOD_NORMAL && game->speed > 80) {
            game->speed -= 2;
        }
    }
    
    if (check_collision(snake)) {
        game->state = STATE_GAME_OVER;
        check_and_save_high_score(game);
    }
}

static bool check_collision(Snake* snake) {
    Position head = snake->segments[0];
    for (int i = 1; i < snake->length; i++) {
        if (snake->segments[i].x == head.x && snake->segments[i].y == head.y) {
            return true;
        }
    }
    return false;
}

static bool check_food_collision(Snake* snake, Food* food) {
    return (snake->segments[0].x == food->pos.x && snake->segments[0].y == food->pos.y);
}