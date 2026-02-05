#include "../include/game.h"
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

/* Globals */
Segment snake[MAX_LEN];
int length = 3;

int foodX, foodY;

Obstacle obstacles[MAX_OBS];
int obstacle_count = 0;

Arrow arrows[MAX_ARROWS];

int score = 0;
Direction dir = RIGHT;

/* Control del juego */
int game_running = 1;

/* Mutex para proteger datos compartidos */
pthread_mutex_t game_mutex = PTHREAD_MUTEX_INITIALIZER;

/* Variable para mensaje de game over */
char game_over_message[100] = "";

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

/*  SOLUCIÓN: No llama endwin() ni printf() directamente */
void game_over(const char* message) {
    pthread_mutex_lock(&game_mutex);
    game_running = 0;
    snprintf(game_over_message, sizeof(game_over_message), "%s", message);
    pthread_mutex_unlock(&game_mutex);
}

// Versión interna sin mutex (para llamar cuando ya tienes el lock)
void spawn_food_internal() {
    int x, y;
    do {
        x = rand() % (WIDTH - 2) + 1;
        y = rand() % (HEIGHT - 2) + 1;
    } while (!is_position_free(x, y));

    foodX = x;
    foodY = y;
}

// Versión pública con mutex (para llamar desde otros hilos)
void spawn_food() {
    pthread_mutex_lock(&game_mutex);
    spawn_food_internal();
    pthread_mutex_unlock(&game_mutex);
}
void spawn_obstacle() {
    pthread_mutex_lock(&game_mutex);
    if (obstacle_count >= MAX_OBS) {
        pthread_mutex_unlock(&game_mutex);
        return;
    }

    int x, y;
    do {
        x = rand() % (WIDTH - 2) + 1;
        y = rand() % (HEIGHT - 2) + 1;
    } while (!is_position_free(x, y) || !space_obstacles(x, y)); // <-- Aquí estaba el error

    obstacles[obstacle_count++] = (Obstacle){x, y};
    pthread_mutex_unlock(&game_mutex);
}


int space_obstacles(int x, int y) {
    // Suponiendo que es el tamaño de tu arreglo
    for (int i = 0; i < obstacle_count; i++) {
        // Comprueba si (x, y) está en el mismo lugar o en los 8 cuadros adyacentes
        if (abs(obstacles[i].x - x) <= 1 && abs(obstacles[i].y - y) <= 1) {
            return 0; // Hay un obstáculo o está demasiado cerca
        }
    }
    return 1; // El espacio está libre y tiene margen
}
/* Flechas */
void spawn_arrow() {
    pthread_mutex_lock(&game_mutex);
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
    pthread_mutex_unlock(&game_mutex);
}

void move_arrows() {
    pthread_mutex_lock(&game_mutex);
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

        /*  ARREGLADO: No llama endwin/printf, solo marca game_running */
        if (arrows[i].x == snake[0].x && arrows[i].y == snake[0].y) {
            game_running = 0;
            snprintf(game_over_message, sizeof(game_over_message), 
                     "Game Over! Hit by arrow ☠  Score: %d", score);
        }
    }
    pthread_mutex_unlock(&game_mutex);
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
    
    // Inicializar todas las flechas como inactivas
    for (int i = 0; i < MAX_ARROWS; i++) {
        arrows[i].active = 0;
    }
}

/* HILO 1: Input (captura de teclas) */
void* input_thread(void* arg) {
    while (game_running) {
        int ch = getch();
        
        pthread_mutex_lock(&game_mutex);
        switch (ch) {
            case KEY_UP: case 'w': 
                if (dir != DOWN) dir = UP; 
                break;
            case KEY_DOWN: case 's': 
                if (dir != UP) dir = DOWN; 
                break;
            case KEY_LEFT: case 'a': 
                if (dir != RIGHT) dir = LEFT; 
                break;
            case KEY_RIGHT: case 'd': 
                if (dir != LEFT) dir = RIGHT; 
                break;
            case 'q': 
                game_running = 0;
                snprintf(game_over_message, sizeof(game_over_message), 
                         "Game quit. Score: %d", score);
                pthread_mutex_unlock(&game_mutex);
                return NULL;
        }
        pthread_mutex_unlock(&game_mutex);
        
        usleep(10000); // 10ms
    }
    return NULL;
}

/* HILO 2: Movimiento de flechas */
void* arrow_thread(void* arg) {
    while (game_running) {
        // Generar flechas aleatoriamente
        if (rand() % 100 < 5) {
            spawn_arrow();
        }
        
        // Mover flechas existentes
        move_arrows();
        
        usleep(100000); // 100ms
    }
    return NULL;
}

/* HILO 3: Generación de obstáculos */
void* obstacle_thread(void* arg) {
    while (game_running) {
        if (rand() % 100 < 3) {
            spawn_obstacle();
        }
        usleep(200000); // 200ms
    }
    return NULL;
}

/* Logic (movimiento de la serpiente) */
void logic() {
    pthread_mutex_lock(&game_mutex);
    
    // Mover cuerpo
    for (int i = length - 1; i > 0; i--)
        snake[i] = snake[i - 1];

    // Mover cabeza
    switch (dir) {
        case UP: snake[0].y--; break;
        case DOWN: snake[0].y++; break;
        case LEFT: snake[0].x--; break;
        case RIGHT: snake[0].x++; break;
    }

    // Colisión con paredes
    if (snake[0].x <= 0 || snake[0].x >= WIDTH ||
        snake[0].y <= 0 || snake[0].y >= HEIGHT) {
        game_running = 0;
        snprintf(game_over_message, sizeof(game_over_message), 
                 "Game Over! Wall hit. Score: %d", score);
        pthread_mutex_unlock(&game_mutex);
        return;
    }

    // Colisión consigo misma
    for (int i = 1; i < length; i++) {
        if (snake[0].x == snake[i].x && snake[0].y == snake[i].y) {
            game_running = 0;
            snprintf(game_over_message, sizeof(game_over_message), 
                     "Game Over! Self hit. Score: %d", score);
            pthread_mutex_unlock(&game_mutex);
            return;
        }
    }

    // Colisión con obstáculos
    for (int i = 0; i < obstacle_count; i++) {
        if (snake[0].x == obstacles[i].x && snake[0].y == obstacles[i].y) {
            game_running = 0;
            snprintf(game_over_message, sizeof(game_over_message), 
                     "Game Over! Obstacle. Score: %d", score);
            pthread_mutex_unlock(&game_mutex);
            return;
        }
    }

    // Comer comida
    if (snake[0].x == foodX && snake[0].y == foodY) {
        length++;
        score += 10;
        spawn_food_internal();  // Usa versión interna (sin mutex)
    }
    
    pthread_mutex_unlock(&game_mutex);
}

