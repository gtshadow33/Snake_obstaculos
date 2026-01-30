#include "../include/game.h"

void draw() {
    clear();

    for (int i = 0; i <= WIDTH; i++) {
        mvprintw(0, i, "#");
        mvprintw(HEIGHT, i, "#");
    }

    for (int i = 0; i <= HEIGHT; i++) {
        mvprintw(i, 0, "#");
        mvprintw(i, WIDTH, "#");
    }

    mvprintw(foodY, foodX, "O");

    for (int i = 0; i < obstacle_count; i++)
        mvprintw(obstacles[i].y, obstacles[i].x, "X");

    for (int i = 0; i < MAX_ARROWS; i++) {
        if (!arrows[i].active) continue;
        char c = (arrows[i].dir == LEFT || arrows[i].dir == RIGHT) ? '-' : '|';
        mvprintw(arrows[i].y, arrows[i].x, "%c", c);
    }

    for (int i = 0; i < length; i++)
        mvprintw(snake[i].y, snake[i].x, i == 0 ? "@" : "o");

    mvprintw(HEIGHT + 1, 0, "Score: %d | Length: %d", score, length);
    mvprintw(HEIGHT + 2, 0, "WASD / Arrows | Q quit");

    refresh();
}
