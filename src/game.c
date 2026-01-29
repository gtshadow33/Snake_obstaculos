#include "../include/game.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <ncurses.h>  // Asegúrate de incluir ncurses

/* Globals */
Segment snake[MAX_LEN];
int length = 3;

int foodX, foodY;

Obstacle obstacles[MAX_OBS];
int obstacle_count = 0;

int score = 0;
Direction dir = RIGHT;

/* Helper function: returns 1 if position is free, 0 if occupied */
int is_position_free(int x, int y) {
    // Check snake
    for (int i = 0; i < length; i++)
        if (snake[i].x == x && snake[i].y == y) return 0;

    // Check obstacles
    for (int i = 0; i < obstacle_count; i++)
        if (obstacles[i].x == x && obstacles[i].y == y) return 0;

    // Check borders
    if (x <= 0 || x >= WIDTH || y <= 0 || y >= HEIGHT) return 0;

    // Check food only if not ignored
    if (foodX == x && foodY == y) return 0;

    return 1;
}

/* Spawn a new obstacle */
void spawn_obstacle() {
    if (obstacle_count >= MAX_OBS) return;

    int x, y, attempts = 0;

    do {
        x = rand() % (WIDTH - 2) + 1;
        y = rand() % (HEIGHT - 2) + 1;
        attempts++;
        if (attempts > 50) return; // Evita bucle infinito
    } while (!is_position_free(x, y));

    obstacles[obstacle_count].x = x;
    obstacles[obstacle_count].y = y;
    obstacle_count++;
}

/* Spawn new food */
void spawn_food() {
    int newX, newY, attempts = 0;
    do {
        newX = rand() % (WIDTH - 2) + 1;
        newY = rand() % (HEIGHT - 2) + 1;
        attempts++;
        if (attempts > 100) return; // Evita bucle infinito
    } while (!is_position_free(newX, newY)); 
    foodX = newX;
    foodY = newY;
}

/* Initialize game */
void init_game() {
    initscr();
    noecho();
    curs_set(FALSE);
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);

    srand(time(NULL));

    // Initialize snake
    for (int i = 0; i < length; i++) {
        snake[i].x = WIDTH / 2 - i;
        snake[i].y = HEIGHT / 2;
    }

    // Initialize food
    spawn_food();

    // Spawn first obstacles
    for (int i = 0; i < 3; i++) {
        spawn_obstacle();
    }
}

/* Handle input */
void input() {
    int ch = getch();

    switch (ch) {
        case KEY_UP: case 'w': case 'W': if (dir != DOWN) dir = UP; break;
        case KEY_DOWN: case 's': case 'S': if (dir != UP) dir = DOWN; break;
        case KEY_LEFT: case 'a': case 'A': if (dir != RIGHT) dir = LEFT; break;
        case KEY_RIGHT: case 'd': case 'D': if (dir != LEFT) dir = RIGHT; break;
        case 'q': case 'Q': endwin(); exit(0);
    }
}

/* Game logic */
void logic() {
    // Occasionally spawn obstacle
    if (rand() % 100 < 3 && obstacle_count < MAX_OBS)
        spawn_obstacle();

    // Move snake body
    for (int i = length - 1; i > 0; i--)
        snake[i] = snake[i - 1];

    // Move head
    switch (dir) {
        case UP: snake[0].y--; break;
        case DOWN: snake[0].y++; break;
        case LEFT: snake[0].x--; break;
        case RIGHT: snake[0].x++; break;
    }

    // Check collisions
    // Border collision
    if (snake[0].x <= 0 || snake[0].x >= WIDTH ||
        snake[0].y <= 0 || snake[0].y >= HEIGHT) {
        endwin();
        printf("Game Over! Hit the wall. Score: %d\n", score);
        exit(0);
    }

    // Self collision
    for (int i = 1; i < length; i++)
        if (snake[0].x == snake[i].x && snake[0].y == snake[i].y) {
            endwin();
            printf("Game Over! Ate yourself. Score: %d\n", score);
            exit(0);
        }

    // Obstacle collision
    for (int i = 0; i < obstacle_count; i++)
        if (snake[0].x == obstacles[i].x && snake[0].y == obstacles[i].y) {
            endwin();
            printf("Game Over! Hit an obstacle. Score: %d\n", score);
            exit(0);
        }

    // Food collision
    if (snake[0].x == foodX && snake[0].y == foodY) {
        length++;
        score += 10;
        spawn_food(); // nueva comida segura
    }
}
