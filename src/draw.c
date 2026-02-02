#include "../include/game.h"

void draw() {
    pthread_mutex_lock(&game_mutex);
    
    //  CAMBIO 1: erase() en lugar de clear()
    erase();
    
    // Dibujar borde
    for (int i = 0; i <= WIDTH; i++) {
        //  CAMBIO 2: mvaddch() en lugar de mvprintw()
        mvaddch(0, i, '#');
        mvaddch(HEIGHT, i, '#');
    }
    for (int i = 0; i <= HEIGHT; i++) {
        mvaddch(i, 0, '#');
        mvaddch(i, WIDTH, '#');
    }
    
    // Dibujar comida
    mvaddch(foodY, foodX, 'O');
    
    // Dibujar obstáculos
    for (int i = 0; i < obstacle_count; i++)
        mvaddch(obstacles[i].y, obstacles[i].x, 'X');
    
    // Dibujar flechas
    for (int i = 0; i < MAX_ARROWS; i++) {
        if (!arrows[i].active) continue;
        char c = (arrows[i].dir == LEFT || arrows[i].dir == RIGHT) ? '-' : '|';
        mvaddch(arrows[i].y, arrows[i].x, c);
    }
    
    // Dibujar serpiente
    for (int i = 0; i < length; i++)
        mvaddch(snake[i].y, snake[i].x, i == 0 ? '@' : 'o');
    
    // Dibujar información (mvprintw está bien para texto)
    mvprintw(HEIGHT + 1, 0, "Score: %d | Length: %d", score, length);
    mvprintw(HEIGHT + 2, 0, "WASD / Arrows | Q quit");
    
    //  CAMBIO 3: refresh() al final (ya lo tenías bien)
    refresh();
    
    pthread_mutex_unlock(&game_mutex);
}