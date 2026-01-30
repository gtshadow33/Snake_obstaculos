#include "../include/game.h"
#include <stdlib.h>
#include <stdio.h>

/* Globals */
Segment snake[MAX_LEN];
int length = 3;

int foodX, foodY;

Obstacle obstacles[MAX_OBS];
int obstacle_count = 0;

Arrow arrows[MAX_ARROWS];

int score = 0;
Direction dir = RIGHT;

/* Helpers */
int is_position_free(int x, int y) {
    for (int i = 0; i < length; i++)
        if (snake[i].x == x && snake[i].y == y) return 0;

    for (int i = 0; i < obstacle_count; i++)
        if (obstacles[i].x == x && obstacles[i].y == y) return 0;

    if (x <= 0 || x >= WIDTH || y <= 0 || y >= HEIGHT) return 0;
    if (foodX == x && foodY == y) return 0;

    return 1;
}

void spawn_food() {
    int x, y;
    do {
        x = rand() % (WIDTH - 2) + 1;
        y = rand() % (HEIGHT - 2) + 1;
    } while (!is_position_free(x, y));

    foodX = x;
    foodY = y;
}

void spawn_obstacle() {
    if (obstacle_count >= MAX_OBS) return;

    int x, y;
    do {
        x = rand() % (WIDTH - 2) + 1;
        y = rand() % (HEIGHT - 2) + 1;
    } while (!is_position_free(x, y));

    obstacles[obstacle_count++] = (Obstacle){x, y};
}

/* Flechas */
void spawn_arrow() {
    for (int i = 0; i < MAX_ARROWS; i++) {
        if (!arrows[i].active) {
            arrows[i].active = 1;
            int side = rand() % 4;

            switch (side) {
                case 0: arrows[i].x = rand() % (WIDTH - 2) + 1; arrows[i].y = 1; arrows[i].dir = DOWN; break;
                case 1: arrows[i].x = rand() % (WIDTH - 2) + 1; arrows[i].y = HEIGHT - 1; arrows[i].dir = UP; break;
                case 2: arrows[i].x = 1; arrows[i].y = rand() % (HEIGHT - 2) + 1; arrows[i].dir = RIGHT; break;
                case 3: arrows[i].x = WIDTH - 1; arrows[i].y = rand() % (HEIGHT - 2) + 1; arrows[i].dir = LEFT; break;
            }
            break;
        }
    }
}

void move_arrows() {
    for (int i = 0; i < MAX_ARROWS; i++) {
        if (!arrows[i].active) continue;

        switch (arrows[i].dir) {
            case UP: arrows[i].y--; break;
            case DOWN: arrows[i].y++; break;
            case LEFT: arrows[i].x--; break;
            case RIGHT: arrows[i].x++; break;
        }

        if (arrows[i].x <= 0 || arrows[i].x >= WIDTH ||
            arrows[i].y <= 0 || arrows[i].y >= HEIGHT) {
            arrows[i].active = 0;
        }

        if (arrows[i].x == snake[0].x && arrows[i].y == snake[0].y) {
            endwin();
            printf("Game Over! Hit by arrow ☠  Score: %d\n", score);
            exit(0);
        }
    }
}

/* Init */
void init_game() {
    initscr();
    noecho();
    curs_set(FALSE);
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);

    srand(time(NULL));

    for (int i = 0; i < length; i++) {
        snake[i].x = WIDTH / 2 - i;
        snake[i].y = HEIGHT / 2;
    }

    spawn_food();

    for (int i = 0; i < 3; i++)
        spawn_obstacle();
}

/* Input */
void input() {
    int ch = getch();
    switch (ch) {
        case KEY_UP: case 'w': if (dir != DOWN) dir = UP; break;
        case KEY_DOWN: case 's': if (dir != UP) dir = DOWN; break;
        case KEY_LEFT: case 'a': if (dir != RIGHT) dir = LEFT; break;
        case KEY_RIGHT: case 'd': if (dir != LEFT) dir = RIGHT; break;
        case 'q': endwin(); exit(0);
    }
}

/* Logic */
void logic() {
    if (rand() % 100 < 5) spawn_arrow();
    if (rand() % 100 < 3) spawn_obstacle();

    move_arrows();

    for (int i = length - 1; i > 0; i--)
        snake[i] = snake[i - 1];

    switch (dir) {
        case UP: snake[0].y--; break;
        case DOWN: snake[0].y++; break;
        case LEFT: snake[0].x--; break;
        case RIGHT: snake[0].x++; break;
    }

    if (snake[0].x <= 0 || snake[0].x >= WIDTH ||
        snake[0].y <= 0 || snake[0].y >= HEIGHT) {
        endwin();
        printf("Game Over! Wall hit. Score: %d\n", score);
        exit(0);
    }

    for (int i = 1; i < length; i++)
        if (snake[0].x == snake[i].x && snake[0].y == snake[i].y) {
            endwin();
            printf("Game Over! Self hit. Score: %d\n", score);
            exit(0);
        }

    for (int i = 0; i < obstacle_count; i++)
        if (snake[0].x == obstacles[i].x && snake[0].y == obstacles[i].y) {
            endwin();
            printf("Game Over! Obstacle. Score: %d\n", score);
            exit(0);
        }

    if (snake[0].x == foodX && snake[0].y == foodY) {
        length++;
        score += 10;
        spawn_food();
    }
}
