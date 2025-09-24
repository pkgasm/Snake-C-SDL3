#include "game.h"
#include <stdio.h>

bool init_game(Game* game) {
    // Inicializar SDL
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        printf("Error inicializando SDL: %s\n", SDL_GetError());
        return false;
    }

    // Inicializar SDL_ttf
    if (!TTF_Init()) {
        printf("Error inicializando SDL_ttf: %s\n", SDL_GetError());
        SDL_Quit();
        return false;
    }

    // Crear ventana
    game->window = SDL_CreateWindow(
        "Snake Game",
        WINDOW_WIDTH, WINDOW_HEIGHT,
        SDL_WINDOW_RESIZABLE
    );
    
    if (!game->window) {
        printf("Error creando ventana: %s\n", SDL_GetError());
        SDL_Quit();
        return false;
    }

    // Crear renderer
    game->renderer = SDL_CreateRenderer(game->window, NULL);
    if (!game->renderer) {
        printf("Error creando renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(game->window);
        SDL_Quit();
        return false;
    }

    // Cargar fuente
    game->font = TTF_OpenFont("Roboto_Condensed-Regular.ttf", 24);
    if (!game->font) {
        printf("Error cargando fuente: %s\n", SDL_GetError());
        SDL_DestroyRenderer(game->renderer);
        SDL_DestroyWindow(game->window);
        SDL_Quit();
        return false;
    }

    return true;
}

void cleanup_game(Game* game) {
    if (game->font) TTF_CloseFont(game->font);
    if (game->renderer) SDL_DestroyRenderer(game->renderer);
    if (game->window) SDL_DestroyWindow(game->window);
    
    TTF_Quit();
    SDL_Quit();
}

void restart_game(Game* game) {
    game->score = 0;
    game->speed = INITIAL_SPEED;
    game->game_won = false;
    game->state = STATE_PLAYING;
}

void load_high_score(Game *game) {
    FILE *file = fopen("highscore.txt", "r");
    if (file) {
        fscanf(file, "%d", &game->high_score);
        fclose(file);
    } else {
        game->high_score = 0;
    }
}

static void save_high_score(int score) {
    FILE *file = fopen("highscore.txt", "w");
    if (file) {
        fprintf(file, "%d", score);
        fclose(file);
    } else {
        printf("Error: No se pudo guardar la puntuacion maxima.\n");
    }
}

void check_and_save_high_score(Game *game) {
    if (game->score > game->high_score) {
        game->high_score = game->score;
        save_high_score(game->high_score);
    }
}