#include "../include/game.h"

void draw() {
    pthread_mutex_lock(&game_mutex);
    
    // NO usar clear() ni erase()
    // En su lugar, dibujar el fondo completo con espacios
    
    // 1. Llenar todo el área de juego con espacios
    for (int y = 1; y < HEIGHT; y++) {
        for (int x = 1; x < WIDTH; x++) {
            mvaddch(y, x, ' ');
        }
    }
    
    // 2. Dibujar borde
    for (int i = 0; i <= WIDTH; i++) {
        mvaddch(0, i, '#');
        mvaddch(HEIGHT, i, '#');
    }
    for (int i = 0; i <= HEIGHT; i++) {
        mvaddch(i, 0, '#');
        mvaddch(i, WIDTH, '#');
    }
    
    // 3. Dibujar comida
    mvaddch(foodY, foodX, 'O');
    
    // 4. Dibujar obstáculos
    for (int i = 0; i < obstacle_count; i++)
        mvaddch(obstacles[i].y, obstacles[i].x, 'X');
    
    // 5. Dibujar flechas
    for (int i = 0; i < MAX_ARROWS; i++) {
        if (!arrows[i].active) continue;
        char c = (arrows[i].dir == LEFT || arrows[i].dir == RIGHT) ? '-' : '|';
        mvaddch(arrows[i].y, arrows[i].x, c);
    }
    
    // 6. Dibujar serpiente
    for (int i = 0; i < length; i++)
        mvaddch(snake[i].y, snake[i].x, i == 0 ? '@' : 'o');
    
    // 7. Información
    mvprintw(HEIGHT + 1, 0, "Score: %d | Length: %d                ", score, length);
    mvprintw(HEIGHT + 2, 0, "WASD / Arrows | Q quit                ");
    
    refresh();
    
    pthread_mutex_unlock(&game_mutex);
}