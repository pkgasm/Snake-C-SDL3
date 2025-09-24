#include "input.h"
#include "game.h" // Para restart_game
#include "snake.h" // Para init_snake y generate_food

void handle_input(Game* game, SDL_Event* event) {
    // --- Eventos globales (se aplican a todos los estados) ---
    if (event->type == SDL_EVENT_QUIT) game->running = false;

    // Movimiento del ratón para detectar hover en botones
    if (event->type == SDL_EVENT_MOUSE_MOTION) {
        SDL_FPoint mouse_pos = { event->motion.x, event->motion.y };
        game->pause_button.hovered = SDL_PointInRectFloat(&mouse_pos, &game->pause_button.rect);
        game->restart_button.hovered = SDL_PointInRectFloat(&mouse_pos, &game->restart_button.rect);
        game->start_button.hovered = SDL_PointInRectFloat(&mouse_pos, &game->start_button.rect);
        game->quit_button.hovered = SDL_PointInRectFloat(&mouse_pos, &game->quit_button.rect);
    }
    
    // --- Eventos específicos por estado ---
    switch (game->state) {
        case STATE_TITLE:
            if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
                if (game->start_button.hovered) {
                    restart_game(game);
                    init_snake(&game->snake);
                    generate_food(game);
                } else if (game->quit_button.hovered) {
                    game->running = false;
                }
            }
            if (event->type == SDL_EVENT_KEY_DOWN && event->key.key == SDLK_RETURN) {
                restart_game(game);
                init_snake(&game->snake);
                generate_food(game);
            }
            break;

        case STATE_PLAYING:
            if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
                if (game->pause_button.hovered) game->state = STATE_PAUSED;
                if (game->restart_button.hovered) {
                    restart_game(game);
                    init_snake(&game->snake);
                    generate_food(game);
                }
            }
            if (event->type == SDL_EVENT_KEY_DOWN) {
                switch (event->key.key) {
                    case SDLK_P: game->state = STATE_PAUSED; break;
                    case SDLK_R:
                        restart_game(game);
                        init_snake(&game->snake);
                        generate_food(game);
                        break;
                    case SDLK_ESCAPE: game->running = false; break;
                    default: { // Movimiento de la serpiente
                        Direction current_dir = game->snake.direction;
                        switch (event->key.key) {
                            case SDLK_UP: case SDLK_W:
                                if (current_dir != DIR_DOWN) game->snake.next_direction = DIR_UP;
                                break;
                            case SDLK_DOWN: case SDLK_S:
                                if (current_dir != DIR_UP) game->snake.next_direction = DIR_DOWN;
                                break;
                            case SDLK_LEFT: case SDLK_A:
                                if (current_dir != DIR_RIGHT) game->snake.next_direction = DIR_LEFT;
                                break;
                            case SDLK_RIGHT: case SDLK_D:
                                if (current_dir != DIR_LEFT) game->snake.next_direction = DIR_RIGHT;
                                break;
                        }
                    }
                }
            }
            break;

        case STATE_PAUSED:
            if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN && game->pause_button.hovered) {
                game->state = STATE_PLAYING;
                game->last_update = SDL_GetTicks();
            }
            if (event->type == SDL_EVENT_KEY_DOWN && event->key.key == SDLK_P) {
                game->state = STATE_PLAYING;
                game->last_update = SDL_GetTicks();
            }
            break;

        case STATE_GAME_OVER:
            if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN && game->restart_button.hovered) {
                restart_game(game);
                init_snake(&game->snake);
                generate_food(game);
            }
            if (event->type == SDL_EVENT_KEY_DOWN) {
                if (event->key.key == SDLK_SPACE) {
                    restart_game(game);
                    init_snake(&game->snake);
                    generate_food(game);
                } else if (event->key.key == SDLK_ESCAPE) {
                    game->running = false;
                }
            }
            break;
    }
}