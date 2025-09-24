#ifndef DEFS_H
#define DEFS_H

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <stdbool.h>

// Constantes del juego
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define GRID_SIZE 20
#define TOP_MARGIN 40 // Espacio para la puntuación
#define GRID_WIDTH (WINDOW_WIDTH / GRID_SIZE)
#define GRID_HEIGHT ((WINDOW_HEIGHT - TOP_MARGIN) / GRID_SIZE)
#define MAX_SNAKE_LENGTH (GRID_WIDTH * GRID_HEIGHT)
#define INITIAL_SPEED 150 // milisegundos

// Colores
#define COLOR_BLACK {0, 0, 0, 255}
#define COLOR_GREEN {0, 255, 0, 255}
#define COLOR_RED {255, 0, 0, 255}
#define COLOR_WHITE {255, 255, 255, 255}
#define COLOR_BLUE {0, 0, 255, 255}
#define COLOR_GOLD {255, 215, 0, 255}
#define COLOR_CYAN {0, 255, 255, 255}

// Direcciones
typedef enum {
    DIR_UP,
    DIR_DOWN,
    DIR_LEFT,
    DIR_RIGHT
} Direction;

// Estructura para posición
typedef struct {
    int x, y;
} Position;

typedef enum {
    STATE_TITLE,
    STATE_PLAYING,
    STATE_PAUSED,
    STATE_GAME_OVER
} GameState;

typedef enum {
    FOOD_NORMAL,        // Comida normal, +10 puntos
    FOOD_SCORE_BOOST,   // Manzana dorada, +50 puntos
    FOOD_SPEED_DOWN     // Cubo de hielo, ralentiza la serpiente
} FoodType;

typedef struct {
    Position pos;
    FoodType type;
} Food;

// Estructura de la serpiente
typedef struct {
    Position segments[MAX_SNAKE_LENGTH];
    int length;
    Direction direction;
    Direction next_direction;
} Snake;

typedef struct {
    SDL_FRect rect;
    char* text;
    bool hovered;
} Button;

// Estructura del juego
typedef struct {
    SDL_Window* window;
    SDL_Renderer* renderer;
    TTF_Font* font;
    Snake snake;
    Food current_food;
    int score;
    bool running;
    GameState state;
    Uint64 last_update;
    Uint64 powerup_end_time;
    int high_score;
    Button start_button;
    Button quit_button;
    Button pause_button;
    Button restart_button;
    bool game_won;
    int speed;
} Game;


#endif // DEFS_H