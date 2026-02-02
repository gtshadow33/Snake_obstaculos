#include "../include/game.h"
#include <unistd.h>
#include <pthread.h>

int main() {
    pthread_t input_t, arrow_t, obstacle_t;
    
    init_game();
    
    // Crear los hilos
    pthread_create(&input_t, NULL, input_thread, NULL);
    pthread_create(&arrow_t, NULL, arrow_thread, NULL);
    pthread_create(&obstacle_t, NULL, obstacle_thread, NULL);
    
    // Hilo principal: lógica y renderizado
    while (game_running) {
        logic();  // Mueve la serpiente
        draw();   // Dibuja todo
        usleep(120000);  // 120ms delay
    }
    
    // Esperar a que terminen los hilos
    pthread_join(input_t, NULL);
    pthread_join(arrow_t, NULL);
    pthread_join(obstacle_t, NULL);
    
    pthread_mutex_destroy(&game_mutex);
    
    endwin();
    return 0;
}