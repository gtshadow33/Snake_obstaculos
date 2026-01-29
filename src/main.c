#include "../include/game.h"
#include <unistd.h>

int main() {
    init_game();

    while (1) {
        input();
        logic();
        draw();
        usleep(120000);
    }

    endwin();
    return 0;
}
