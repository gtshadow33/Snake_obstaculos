#ifndef GAME_H
#define GAME_H

#include <ncurses.h>
#include <time.h>

#define WIDTH 40
#define HEIGHT 20
#define MAX_LEN 100
#define MAX_OBS 20
#define MAX_ARROWS 30

typedef struct {
    int x;
    int y;
} Segment;

typedef struct {
    int x;
    int y;
} Obstacle;

typedef enum { UP, DOWN, LEFT, RIGHT } Direction;

typedef struct {
    int x, y;
    Direction dir;
    int active;
} Arrow;

/* Snake */
extern Segment snake[MAX_LEN];
extern int length;

/* Food */
extern int foodX, foodY;

/* Obstacles */
extern Obstacle obstacles[MAX_OBS];
extern int obstacle_count;

/* Arrows */
extern Arrow arrows[MAX_ARROWS];

/* Game state */
extern int score;
extern Direction dir;

/* Functions */
void init_game();
void input();
void logic();
void draw();

#endif
