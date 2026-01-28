#ifndef ENEMY_H
#define ENEMY_H
#include "raylib.h"

    #define ENEMY_NUM 25

    typedef struct{
        Vector2 pos;
        Rectangle rec;
        Vector2 dir;


        bool active;
        float speed;
        
    }Enemy;

    void enemyInit(Enemy enemy[], Vector2 playerPos);

    void enemyUpdate(Enemy enemy[], Vector2 playerPos, Rectangle axeRec);

    void enemyDraw(Enemy enemy[]);

    void enemyDelete(Enemy enemy[], int i);




#endif