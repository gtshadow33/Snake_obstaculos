#ifndef GAME_H
#define GAME_H

#include <ncurses.h>
#include <time.h>
#include <pthread.h>

#define WIDTH 40
#define HEIGHT 20
#define MAX_LEN 100
#define MAX_OBS 20
#define MAX_ARROWS 10

typedef enum { UP, DOWN, LEFT, RIGHT } Direction;

typedef struct {
    int x, y;
} Segment;

typedef struct {
    int x, y;
} Obstacle;

typedef struct {
    int x, y;
    Direction dir;
    int active;
} Arrow;

/* Globales */
extern Segment snake[MAX_LEN];
extern int length;
extern int foodX, foodY;
extern Obstacle obstacles[MAX_OBS];
extern int obstacle_count;
extern Arrow arrows[MAX_ARROWS];
extern int score;
extern Direction dir;
extern int game_running;

/* Mutex para sincronización */
extern pthread_mutex_t game_mutex;

/* Funciones */
void init_game();
void input();
void logic();
void draw();
void spawn_food();
void spawn_obstacle();
void spawn_arrow();
void move_arrows();

/* Funciones de hilos */
void* input_thread(void* arg);
void* arrow_thread(void* arg);
void* obstacle_thread(void* arg);

#endif