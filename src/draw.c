/* Draw con doble buffer (sin parpadeo) */
void draw() {

    
    // En lugar de clear(), usar erase() que no causa refresh automático
    erase();
    
    // Dibujar borde
    for (int i = 0; i < WIDTH; i++) {
        mvaddch(0, i, '#');
        mvaddch(HEIGHT, i, '#');
    }
    for (int i = 0; i < HEIGHT; i++) {
        mvaddch(i, 0, '#');
        mvaddch(i, WIDTH, '#');
    }
    
    // Dibujar comida
    mvaddch(foodY, foodX, '@');
    
    // Dibujar serpiente
    for (int i = 0; i < length; i++) {
        if (i == 0)
            mvaddch(snake[i].y, snake[i].x, 'O');  // Cabeza
        else
            mvaddch(snake[i].y, snake[i].x, 'o');  // Cuerpo
    }
    
    // Dibujar obstáculos
    for (int i = 0; i < obstacle_count; i++) {
        mvaddch(obstacles[i].y, obstacles[i].x, 'X');
    }
    
    // Dibujar flechas
    for (int i = 0; i < MAX_ARROWS; i++) {
        if (arrows[i].active) {
            char arrow_char;
            switch (arrows[i].dir) {
                case UP: arrow_char = '^'; break;
                case DOWN: arrow_char = 'v'; break;
                case LEFT: arrow_char = '<'; break;
                case RIGHT: arrow_char = '>'; break;
            }
            mvaddch(arrows[i].y, arrows[i].x, arrow_char);
        }
    }
    
    // Dibujar score
    mvprintw(HEIGHT + 1, 0, "Score: %d | Length: %d | Press 'q' to quit", score, length);
    
    // Actualizar pantalla solo UNA VEZ al final
    refresh();
    
    pthread_mutex_unlock(&game_mutex);
}