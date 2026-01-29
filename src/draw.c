#include "../include/game.h"

void draw() {
    clear();

    /* Draw borders */
    for (int i = 0; i <= WIDTH; i++) {
        mvprintw(0, i, "#");
        mvprintw(HEIGHT, i, "#");
    }

    for (int i = 0; i <= HEIGHT; i++) {
        mvprintw(i, 0, "#");
        mvprintw(i, WIDTH, "#");
    }

    /* Draw food */
    mvprintw(foodY, foodX, "O");

    /* Draw obstacles */
    for (int i = 0; i < obstacle_count; i++) {
        mvprintw(obstacles[i].y, obstacles[i].x, "X");
    }

    /* Draw snake */
    for (int i = 0; i < length; i++) {
        mvprintw(snake[i].y, snake[i].x, i == 0 ? "@" : "o");
    }

    /* Draw score and info */
    mvprintw(HEIGHT + 1, 0, "Score: %d | Length: %d | Obstacles: %d", score, length, obstacle_count);
    mvprintw(HEIGHT + 2, 0, "Controls: WASD/Arrows | Quit: Q");
    
    refresh();
}