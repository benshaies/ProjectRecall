#ifndef ARENA_H
#define ARENA_H
#include "raylib.h"

#define TILE_SIZE 100
#define LEVEL_WIDTH 31
#define LEVEL_HEIGHT 34

#define FLOOR_WIDTH 31
#define FlOOR_HEIGHT 15

    

    void csvToArray(int array[][LEVEL_WIDTH], const char *filename);

    void drawLevel(int array[][LEVEL_WIDTH], int layer);

    void drawFloor(int array[][FLOOR_WIDTH]);

    bool isWallTile(int tileId);

    int getWallAmount(int array[][LEVEL_WIDTH]);

    void createCollisionRecs(int array[][LEVEL_WIDTH], Rectangle rec[]);

#endif
