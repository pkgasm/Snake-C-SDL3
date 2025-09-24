#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "defs.h"
#include "game.h"
#include "input.h"
#include "render.h"
#include "snake.h"

// Funciones
void game_loop(Game* game);

void game_loop(Game* game) {
    const int TARGET_FPS = 60;
    const int FRAME_DELAY = 1000 / TARGET_FPS;
    Uint64 frame_start;
    int frame_time;
    
    while (game->running) {
        frame_start = SDL_GetTicks();

        // Manejar eventos
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            handle_input(game, &event);
        }
        
        // Actualizar juego
        if (game->state == STATE_PLAYING) {
            Uint64 current_time = SDL_GetTicks();
            // Restaurar velocidad si el power-up ha terminado
            if (game->powerup_end_time > 0 && current_time >= game->powerup_end_time) {
                game->speed = INITIAL_SPEED - (game->score / 10) * 2; // Reajustar velocidad según puntuación
                if (game->speed < 80) game->speed = 80;
                game->powerup_end_time = 0;
            }
            if (current_time - game->last_update >= game->speed) {
                update_snake(game);
                game->last_update = current_time;
            }
        }
        
        // Renderizar
        render_game(game);
        
        // Limitar FPS
        frame_time = SDL_GetTicks() - frame_start;
        if (FRAME_DELAY > frame_time) {
            SDL_Delay(FRAME_DELAY - frame_time);
        }
    }
}

int main(int argc, char* argv[]) {
    srand(time(NULL));
    
    Game game = {0};
    game.running = true;
    game.state = STATE_TITLE;
    game.game_won = false;
    game.score = 0;
    game.speed = INITIAL_SPEED;

    // Inicializar botones de juego
    game.pause_button.rect = (SDL_FRect){WINDOW_WIDTH - 240, 5, 100, 30};
    game.pause_button.hovered = false;
    game.restart_button.rect = (SDL_FRect){WINDOW_WIDTH - 130, 5, 120, 30};
    game.restart_button.text = "Reiniciar";
    game.restart_button.hovered = false;

    // Inicializar botones de menú
    game.start_button = (Button){{(WINDOW_WIDTH - 200) / 2, WINDOW_HEIGHT / 2 - 20, 200, 50}, "Iniciar Juego", false};
    game.quit_button = (Button){{(WINDOW_WIDTH - 200) / 2, WINDOW_HEIGHT / 2 + 40, 200, 50}, "Salir", false};
    
    // Cargar puntuación máxima
    load_high_score(&game);

    // Inicializar SDL y crear ventana
    if (!init_game(&game)) {
        printf("Error inicializando el juego\n");
        return 1;
    }
    
    // Inicializar serpiente y comida
    // Se mueven a cuando se inicia el juego desde el menú
    // init_snake(&game.snake);
    // generate_food(&game);
    
    printf("Controles:\n");
    printf("- Flechas o WASD: Mover serpiente\n");
    printf("- P: Pausar / Reanudar\n");
    printf("- Clic en los botones de la interfaz\n");
    printf("- R: Reiniciar juego\n");
    printf("- ESC: Salir\n");
    printf("- ESPACIO: Reiniciar (cuando termine el juego)\n");
    
    // Ejecutar juego
    game_loop(&game);
    
    // Limpiar recursos
    cleanup_game(&game);
    
    printf("¡Gracias por jugar!\n");
    return 0;
}