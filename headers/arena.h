#ifndef ARENA_H
#define ARENA_H
#include "raylib.h"

#define TILE_SIZE 100
#define LEVEL_WIDTH 45
#define LEVEL_HEIGHT 37

    

    void csvToArray(int array[][LEVEL_WIDTH], const char *filename);

    void drawLevel(int array[][LEVEL_WIDTH]);

#endif
