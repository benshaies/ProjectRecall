#ifndef ENEMY_H
#define ENEMY_H
#include "raylib.h"

    #define ENEMY_NUM 25

    typedef enum{
        HIT,
        NOT_HIT,
    }enemyState;

    typedef struct{
        Vector2 pos;
        Rectangle rec;
        Vector2 dir;
        bool active;
        float speed;
        enemyState state;
        int health;
        float baseHealth;
        
    }Enemy;

    void enemyInit(Enemy enemy[], Vector2 playerPos);

    void enemyUpdate(Enemy enemy[], Rectangle playerRec, Rectangle axeRec, float currentAxeDamage);


    void enemyDraw(Enemy enemy[]);

    void enemyDelete(Enemy enemy[], int i);




#endif