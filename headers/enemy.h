#ifndef ENEMY_H
#define ENEMY_H
#include "raylib.h"
#include "player.h"

    #define ENEMY_NUM 25

    typedef enum{
        HIT,
        NOT_HIT,
        IMMUNE,
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
        Vector2 knockbackDir;
        Color color;
        int hitFrameCount;
        
    }Enemy;

    void enemyInit(Enemy enemy[], Vector2 playerPos);

    bool enemyUpdate(Enemy enemy[], Rectangle playerRec, Weapon axe);

    void enemyDraw(Enemy enemy[]);

    void enemyDelete(Enemy enemy[], int i);




#endif