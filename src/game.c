#include "../include/game.h"
#include <stdlib.h>
#include <stdio.h>

/* Globals */
Segment snake[MAX_LEN];
int length = 3;

int foodX, foodY;

Obstacle obstacles[MAX_OBS];
int obstacle_count = 0;
time_t last_obstacle_time;

int score = 0;
Direction dir = RIGHT;

/* Helpers */
int position_free(int x, int y) {
    for (int i = 0; i < length; i++)
        if (snake[i].x == x && snake[i].y == y)
            return 0;

    if (foodX == x && foodY == y)
        return 0;

    for (int i = 0; i < obstacle_count; i++)
        if (obstacles[i].x == x && obstacles[i].y == y)
            return 0;

    return 1;
}

void spawn_obstacle() {
    if (obstacle_count >= MAX_OBS) return;

    int x, y;
    do {
        x = rand() % (WIDTH - 2) + 1;
        y = rand() % (HEIGHT - 2) + 1;
    } while (!position_free(x, y));

    obstacles[obstacle_count].x = x;
    obstacles[obstacle_count].y = y;
    obstacle_count++;
}

/* Game */
void init_game() {
    initscr();
    noecho();
    curs_set(FALSE);
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);

    srand(time(NULL));
    last_obstacle_time = time(NULL);

    for (int i = 0; i < length; i++) {
        snake[i].x = WIDTH / 2 - i;
        snake[i].y = HEIGHT / 2;
    }

    foodX = rand() % (WIDTH - 2) + 1;
    foodY = rand() % (HEIGHT - 2) + 1;
}

void input() {
    int ch = getch();

    switch (ch) {
        case KEY_UP:
        case 'w': if (dir != DOWN) dir = UP; break;
        case KEY_DOWN:
        case 's': if (dir != UP) dir = DOWN; break;
        case KEY_LEFT:
        case 'a': if (dir != RIGHT) dir = LEFT; break;
        case KEY_RIGHT:
        case 'd': if (dir != LEFT) dir = RIGHT; break;
        case 'q':
            endwin();
            exit(0);
    }
}

void logic() {
    time_t now = time(NULL);
    if (difftime(now, last_obstacle_time) >= 10) {
        spawn_obstacle();
        last_obstacle_time = now;
    }

    for (int i = length - 1; i > 0; i--) {
        snake[i] = snake[i - 1];
    }

    switch (dir) {
        case UP:    snake[0].y--; break;
        case DOWN:  snake[0].y++; break;
        case LEFT:  snake[0].x--; break;
        case RIGHT: snake[0].x++; break;
    }

    if (snake[0].x <= 0 || snake[0].x >= WIDTH ||
        snake[0].y <= 0 || snake[0].y >= HEIGHT) {
        endwin();
        printf("Game Over! Score: %d\n", score);
        exit(0);
    }

    for (int i = 1; i < length; i++) {
        if (snake[0].x == snake[i].x &&
            snake[0].y == snake[i].y) {
            endwin();
            printf("Game Over! Score: %d\n", score);
            exit(0);
        }
    }

    for (int i = 0; i < obstacle_count; i++) {
        if (snake[0].x == obstacles[i].x &&
            snake[0].y == obstacles[i].y) {
            endwin();
            printf("Game Over! Chocaste con un obstáculo   Score: %d\n", score);
            exit(0);
        }
    }

    if (snake[0].x == foodX && snake[0].y == foodY) {
        length++;
        score += 10;

        do {
            foodX = rand() % (WIDTH - 2) + 1;
            foodY = rand() % (HEIGHT - 2) + 1;
        } while (!position_free(foodX, foodY));
    }
}
