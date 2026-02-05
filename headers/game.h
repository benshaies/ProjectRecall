#ifndef GAME_H
#define GAME_H
#include "raylib.h"
#include "arena.h"

    extern Vector2 mousePos;
    extern Vector2 worldMouse;

    typedef struct{
        float enemySpawnTimer;
        int levelArray[LEVEL_HEIGHT][LEVEL_WIDTH];
    }Game;

    

    void gameInit();

    void gameSetFullscreen();

    void gameUpdate();

    void gameResolutionDraw();

    void gameDraw();

    void spawnEnemies();

#endif