#  Snake Game - Multihilo en C

Un juego de Snake clásico implementado en C con **arquitectura multihilo** y renderizado en tiempo real usando ncurses. El juego demuestra conceptos avanzados de programación concurrente, sincronización con mutex y gestión de recursos en C.

## 🎮 Características

- **Serpiente controlable** con WASD o flechas del teclado
- **Comida (O)** que aparece aleatoriamente en el tablero
- **Obstáculos (X)** que se generan dinámicamente
- **Flechas (- |)** que atraviesan la pantalla y terminan el juego si golpean
- **Sistema de puntuación** que aumenta con cada comida consumida
- **Arquitectura multihilo** con sincronización segura mediante mutex

##  Arquitectura Multihilo

El juego utiliza **4 hilos concurrentes** para máxima eficiencia:

### Hilo Principal
- Ejecuta la lógica del juego (movimiento de serpiente, colisiones)
- Renderiza la pantalla
- Itera cada 120ms

### Hilo de Entrada (input_thread)
- Captura eventos del teclado en tiempo real
- Responde a WASD, flechas y tecla 'Q' para salir
- No bloquea el flujo principal del juego

### Hilo de Flechas (arrow_thread)
- Genera flechas aleatorias desde los bordes (5% de probabilidad)
- Mueve flechas activas por el tablero (100ms)
- Detecta colisión con la cabeza de la serpiente

### Hilo de Obstáculos (obstacle_thread)
- Genera obstáculos aleatorios (3% de probabilidad)
- Evita colisiones con serpiente, comida y otros obstáculos
- Actualiza cada 200ms

##  Sincronización

Todo acceso a datos compartidos está protegido por un **mutex** (`pthread_mutex_t`):

```c
typedef struct {
    int x, y;
} Segment;

typedef struct {
    int x, y;
    Direction dir;
    int active;
} Arrow;

/* Datos compartidos protegidos por mutex */
extern Segment snake[MAX_LEN];
extern Arrow arrows[MAX_ARROWS];
extern Obstacle obstacles[MAX_OBS];
```

**Funciones críticas:**
- `spawn_food()` - versión con mutex para hilos
- `spawn_food_internal()` - versión sin mutex para lógica principal
- `move_arrows()` - genera colisiones con sincronización
- `logic()` - detecta todas las colisiones dentro del lock

##  Mecánicas del Juego

### Colisiones Detectadas
1. **Paredes** - El juego termina
2. **Autocholisión** - La serpiente choca consigo misma
3. **Obstáculos** - Colisión con X
4. **Flechas** - Golpeadas por proyectiles
5. **Comida** - Aumenta longitud y puntuación

### Dimensiones
- **Ancho:** 40 caracteres
- **Alto:** 20 líneas
- **Máx. longitud serpiente:** 100 segmentos
- **Máx. obstáculos:** 20
- **Máx. flechas activas:** 10

##  Requisitos

```bash
# Ubuntu/Debian
sudo apt-get install libncurses-dev cmake build-essential

# macOS (Homebrew)
brew install ncurses cmake
```

## 🛠️ Compilación

```bash
# Con CMake
mkdir build
cd build
cmake ..
make

# O compilar directamente
gcc -Wall -Wextra -I./include -pthread -lncurses src/*.c -o snake
```

##  Ejecución

```bash
./snake
```

## 🎮 Controles

| Tecla | Acción |
|-------|--------|
| **W** / **↑** | Arriba |
| **A** / **←** | Izquierda |
| **S** / **↓** | Abajo |
| **D** / **→** | Derecha |
| **Q** | Salir del juego |

##  Sistema de Puntuación

- **+10 puntos** por cada comida consumida
- La longitud aumenta proporcionalmente
- El juego termina al colisionar con cualquier obstáculo

##  Estructura del Proyecto

```
.
├── CMakeLists.txt          # Configuración de compilación
├── include/
│   └── game.h              # Definiciones y tipos
├── src/
│   ├── main.c              # Punto de entrada y bucle principal
│   ├── game.c              # Lógica de juego y threads
│   └── draw.c              # Renderizado con ncurses
└── README.md               # Este archivo
```

##  Puntos Técnicos Clave

### 1. Mutex para Protección
```c
pthread_mutex_lock(&game_mutex);
// Código crítico
pthread_mutex_unlock(&game_mutex);
```

### 2. Inicialización Segura
```c
// Inicializar ncurses ANTES de crear hilos
initscr();
noecho();
curs_set(FALSE);
nodelay(stdscr, TRUE);
```

### 3. Limpieza Correcta
```c
// Esperar hilos ANTES de endwin()
pthread_join(input_t, NULL);
pthread_join(arrow_t, NULL);
pthread_join(obstacle_t, NULL);

// endwin() al final
endwin();
```

### 4. Detección de Colisiones Sincronizada
```c
for (int i = 0; i < obstacle_count; i++) {
    if (snake[0].x == obstacles[i].x && snake[0].y == obstacles[i].y) {
        game_running = 0; // Termina juego de forma segura
    }
}
```

##  Manejo de Game Over

Todas las condiciones de fin de juego establecen `game_running = 0` dentro del mutex, lo que permite que el bucle principal termine ordenadamente sin conflictos con otros hilos.

##  Conceptos Educativos

Este proyecto demuestra:
-  Programación multihilo en C
-  Sincronización con mutex
-  Manejo de datos compartidos
-  Race conditions y cómo evitarlas
-  Programación defensiva en ncurses
-  Gestión de recursos y limpieza
-  Diseño de arquitectura concurrente

##  Notas de Implementación

- **Sin `clear()` ni `erase()`**: El draw llena manualmente con espacios para evitar problemas de renderizado
- **nodelay(TRUE)**: Permite que `getch()` sea no-bloqueante
- **Sincronización de acceso**: Todo acceso a estructuras compartidas está protegido
- **Spawn seguro**: `spawn_food_internal()` para código que ya tiene el lock

## Solución de Problemas

### "error: conflicting types for 'input'"
Asegúrate de que todas las funciones estén declaradas en `game.h`

### Juego lento o entrecortado
Ajusta los valores `usleep()` en cada hilo (valores en microsegundos):
- Main loop: `120000` (120ms)
- Input thread: `10000` (10ms)
- Arrow thread: `100000` (100ms)
- Obstacle thread: `200000` (200ms)

### ncurses no renderiza correctamente
Verifica que `endwin()` se llame SOLO después de que todos los hilos terminen

##  Licencia

Este proyecto es de código abierto para propósitos educativos.

##  Contribuciones

¡Las mejoras son bienvenidas! Algunos ideas:
- Modos de dificultad progresiva
- Powerups especiales
- Modo multijugador local
- Tabla de máximas puntuaciones

---

**Hecho con  en C | Programación Concurrente**