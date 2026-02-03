cmake_minimum_required(VERSION 3.10)

project(snake C)

# Compilador (opcional, normalmente no hace falta)
set(CMAKE_C_COMPILER gcc)

# Estándar de C (opcional pero recomendado)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Archivos fuente
set(SOURCES
    src/main.c
    src/game.c
    src/draw.c
)

# Ejecutable
add_executable(snake ${SOURCES})

# Includes (-Iinclude)
target_include_directories(snake PRIVATE include)

# Flags (-Wall -Wextra)
target_compile_options(snake PRIVATE -Wall -Wextra)

# Librería ncurses (-lncurses)
target_link_libraries(snake ncurses)
