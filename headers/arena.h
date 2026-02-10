#ifndef ARENA_H
#define ARENA_H
#include "raylib.h"

#define TILE_SIZE 100
#define LEVEL_WIDTH 31
#define LEVEL_HEIGHT 34

    

    void csvToArray(int array[][LEVEL_WIDTH], const char *filename);

    void drawLevel(int array[][LEVEL_WIDTH]);

#endif
