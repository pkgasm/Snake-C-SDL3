#include "render.h"
#include <stdio.h>

void render_text(Game* game, const char* text, float x, float y, SDL_Color color, bool centered, const SDL_FRect* container) {
    SDL_Surface* surface = TTF_RenderText_Solid(game->font, text, 0, color);
    if (!surface) return;
    
    SDL_Texture* texture = SDL_CreateTextureFromSurface(game->renderer, surface);
    if (!texture) {
        SDL_DestroySurface(surface);
        return;
    }

    SDL_FRect dest_rect = { x, y, (float)surface->w, (float)surface->h };
    if (centered && container) {
        dest_rect.x = container->x + (container->w - dest_rect.w) / 2.0f;
        dest_rect.y = container->y + (container->h - dest_rect.h) / 2.0f;
    }
    SDL_RenderTexture(game->renderer, texture, NULL, &dest_rect);
    
    SDL_DestroyTexture(texture);
    SDL_DestroySurface(surface);
}

static void render_title_screen(Game* game) {
    SDL_Color white = COLOR_WHITE;

    render_text(game, "SNAKE", WINDOW_WIDTH / 2 - 100, WINDOW_HEIGHT / 4, white, false, NULL);

    // Botón de Iniciar
    SDL_Color start_btn_color = game->start_button.hovered ? (SDL_Color){150, 250, 150, 255} : (SDL_Color){100, 200, 100, 255};
    SDL_SetRenderDrawColor(game->renderer, start_btn_color.r, start_btn_color.g, start_btn_color.b, start_btn_color.a);
    SDL_RenderFillRect(game->renderer, &game->start_button.rect);
    render_text(game, game->start_button.text, 0, 0, white, true, &game->start_button.rect);

    // Botón de Salir
    SDL_Color quit_btn_color = game->quit_button.hovered ? (SDL_Color){250, 150, 150, 255} : (SDL_Color){200, 100, 100, 255};
    SDL_SetRenderDrawColor(game->renderer, quit_btn_color.r, quit_btn_color.g, quit_btn_color.b, quit_btn_color.a);
    SDL_RenderFillRect(game->renderer, &game->quit_button.rect);
    render_text(game, game->quit_button.text, 0, 0, white, true, &game->quit_button.rect);
}

static void render_playing_screen(Game* game) {
    SDL_Color green = COLOR_GREEN;
    SDL_Color red = COLOR_RED;
    SDL_Color gold = COLOR_GOLD;
    SDL_Color cyan = COLOR_CYAN;
    SDL_Color blue = COLOR_BLUE;

    // Dibujar serpiente
    SDL_SetRenderDrawColor(game->renderer, green.r, green.g, green.b, green.a);
    for (int i = 0; i < game->snake.length; i++) {
        SDL_FRect rect = { game->snake.segments[i].x * GRID_SIZE, game->snake.segments[i].y * GRID_SIZE + TOP_MARGIN, GRID_SIZE - 1, GRID_SIZE - 1 };
        SDL_RenderFillRect(game->renderer, &rect);
    }
    
    // Dibujar cabeza de diferente color
    SDL_SetRenderDrawColor(game->renderer, blue.r, blue.g, blue.b, blue.a);
    SDL_FRect head_rect = { game->snake.segments[0].x * GRID_SIZE, game->snake.segments[0].y * GRID_SIZE + TOP_MARGIN, GRID_SIZE - 1, GRID_SIZE - 1 };
    SDL_RenderFillRect(game->renderer, &head_rect);
    
    // Dibujar comida
    SDL_Color food_color = red;
    switch (game->current_food.type) {
        case FOOD_NORMAL:
            food_color = red;
            break;
        case FOOD_SCORE_BOOST:
            food_color = gold;
            break;
        case FOOD_SPEED_DOWN:
            food_color = cyan;
            break;
    }
    SDL_SetRenderDrawColor(game->renderer, food_color.r, food_color.g, food_color.b, food_color.a);
    SDL_FRect food_rect = { game->current_food.pos.x * GRID_SIZE, game->current_food.pos.y * GRID_SIZE + TOP_MARGIN, GRID_SIZE - 1, GRID_SIZE - 1 };
    SDL_RenderFillRect(game->renderer, &food_rect);
}

void render_game(Game* game) {
    SDL_Color black = COLOR_BLACK;
    SDL_Color green = COLOR_GREEN;
    SDL_Color red = COLOR_RED;
    SDL_Color white = COLOR_WHITE;
    
    // Limpiar pantalla
    SDL_SetRenderDrawColor(game->renderer, black.r, black.g, black.b, black.a);
    SDL_RenderClear(game->renderer);

    if (game->state == STATE_TITLE) {
        render_title_screen(game);
    } else { // Para STATE_PLAYING, STATE_PAUSED, STATE_GAME_OVER
        // Dibujar el borde del área de juego
        SDL_SetRenderDrawColor(game->renderer, white.r, white.g, white.b, white.a);
        SDL_FRect border_rect = {0, TOP_MARGIN, WINDOW_WIDTH, WINDOW_HEIGHT - TOP_MARGIN};
        SDL_RenderRect(game->renderer, &border_rect);

        // Dibujar puntuación
        char score_text[50];
        snprintf(score_text, sizeof(score_text), "Puntuacion: %d | Maxima: %d", game->score, game->high_score);
        render_text(game, score_text, 10, 10, white, false, NULL);

        // --- Dibujar Botones de juego ---
        SDL_Color pause_btn_color = game->pause_button.hovered ? (SDL_Color){150, 150, 250, 255} : (SDL_Color){100, 100, 200, 255};
        SDL_SetRenderDrawColor(game->renderer, pause_btn_color.r, pause_btn_color.g, pause_btn_color.b, pause_btn_color.a);
        SDL_RenderFillRect(game->renderer, &game->pause_button.rect);
        game->pause_button.text = (game->state == STATE_PAUSED) ? "Seguir" : "Pausa";
        render_text(game, game->pause_button.text, 0, 0, white, true, &game->pause_button.rect);

        SDL_Color restart_btn_color = game->restart_button.hovered ? (SDL_Color){250, 150, 150, 255} : (SDL_Color){200, 100, 100, 255};
        SDL_SetRenderDrawColor(game->renderer, restart_btn_color.r, restart_btn_color.g, restart_btn_color.b, restart_btn_color.a);
        SDL_RenderFillRect(game->renderer, &game->restart_button.rect);
        render_text(game, game->restart_button.text, 0, 0, white, true, &game->restart_button.rect);

        if (game->state == STATE_PLAYING) {
            render_playing_screen(game);
        } else if (game->state == STATE_PAUSED) {
            render_playing_screen(game); // Dibuja el juego congelado
            render_text(game, "PAUSA", WINDOW_WIDTH/2 - 40, WINDOW_HEIGHT/2 - 50, white, false, NULL);
            render_text(game, "Presiona P o el boton para continuar", WINDOW_WIDTH/2 - 180, WINDOW_HEIGHT/2 - 10, white, false, NULL);
        } else if (game->state == STATE_GAME_OVER) {
            render_playing_screen(game); // Dibuja el estado final del juego
            if (game->game_won) {
                render_text(game, "¡GANASTE!", WINDOW_WIDTH/2 - 80, WINDOW_HEIGHT/2 - 50, green, false, NULL);
                render_text(game, "Presiona ESPACIO o el boton para reiniciar", WINDOW_WIDTH/2 - 200, WINDOW_HEIGHT/2 - 10, white, false, NULL);
            } else {
                render_text(game, "GAME OVER", WINDOW_WIDTH/2 - 80, WINDOW_HEIGHT/2 - 50, red, false, NULL);
                render_text(game, "Presiona ESPACIO o el boton para reiniciar", WINDOW_WIDTH/2 - 200, WINDOW_HEIGHT/2 - 10, white, false, NULL);
                render_text(game, "o ESC para salir", WINDOW_WIDTH/2 - 80, WINDOW_HEIGHT/2 + 20, white, false, NULL);
            }
        }
    }
    
    // Presentar frame
    SDL_RenderPresent(game->renderer);
}