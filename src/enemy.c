#include "../headers/enemy.h"
#include "raymath.h"
#include "stdio.h"

void enemyInit(Enemy enemy[], Vector2 playerPos){
    for (int i = 0; i < ENEMY_NUM; i++){
        if(!enemy[i].active){
            enemy[i].pos = (Vector2){GetRandomValue(0, 1280), GetRandomValue(0, 720)};
            enemy[i].rec = (Rectangle){enemy[i].pos.x, enemy[i].pos.y, 50, 50};
            enemy[i].speed = 1.0f;
            enemy[i].dir = Vector2Normalize((Vector2){playerPos.x - enemy[i].pos.x, playerPos.y - enemy[i].pos.y});
            enemy[i].active = true;

            break;
        }
    }
}

void enemyUpdate(Enemy enemy[], Vector2 playerPos, Rectangle axeRec){
    for(int i = 0; i < ENEMY_NUM; i++){
        if(enemy[i].active){
            enemy[i].dir = Vector2Normalize((Vector2){playerPos.x - enemy[i].pos.x, playerPos.y - enemy[i].pos.y});
            enemy[i].pos.x += enemy[i].dir.x * enemy[i].speed;
            enemy[i].pos.y += enemy[i].dir.y * enemy[i].speed;
            enemy[i].rec = (Rectangle){enemy[i].pos.x, enemy[i].pos.y, 50, 50};
        

            //Axe collision
            if(CheckCollisionRecs(enemy[i].rec, axeRec)){
                 enemyDelete(enemy, i);
                 break;
            }

        
        }
    }
}

void enemyDraw(Enemy enemy[]){
    for(int i = 0; i < ENEMY_NUM; i++){
        if(enemy[i].active){
            DrawRectangleRec(enemy[i].rec, RED);
        }
    }
}

void enemyDelete(Enemy enemy[], int i){
    enemy[i].active = false;
    enemy[i].dir = (Vector2){0,0};
    enemy[i].pos = (Vector2){0,0};
    enemy[i].rec = (Rectangle){0,0,0,0};
    enemy[i].speed = 0;
}
