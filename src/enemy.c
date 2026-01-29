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
            enemy[i].health = 100;
            enemy[i].baseHealth = 100.0f;
            enemy[i].state = NOT_HIT;

            break;
        }
    }
}

void enemyUpdate(Enemy enemy[], Rectangle playerRec, Rectangle axeRec, float currentAxeDamage){
    for(int i = 0; i < ENEMY_NUM; i++){
        if(enemy[i].active){

            enemy[i].dir = Vector2Normalize((Vector2){playerRec.x - enemy[i].pos.x, playerRec.y - enemy[i].pos.y});
            enemy[i].pos.x += enemy[i].dir.x * enemy[i].speed;
            enemy[i].pos.y += enemy[i].dir.y * enemy[i].speed;
            enemy[i].rec = (Rectangle){enemy[i].pos.x, enemy[i].pos.y, 50, 50};
        

            //Enemy hit check (Only acitvates if enemy isnt currently hit)
            if(CheckCollisionRecs(enemy[i].rec, axeRec) && enemy[i].state == NOT_HIT){
                 enemy[i].health -= currentAxeDamage;
                 printf("%d\n", enemy[i].health);
                 enemy[i].state = HIT;
                 //break;
            }

            //If axe and enemy not colliding, resets state
            if(!CheckCollisionRecs(enemy[i].rec,axeRec)){
                enemy[i].state = NOT_HIT;
            }

            //Remove enemy once health is 0
            if(enemy[i].health <= 0){
                enemyDelete(enemy, i);
            }

            //Check Collision with player
            if(CheckCollisionRecs(enemy[i].rec, playerRec)){
                CloseWindow();
            }

        
        }
    }
}

void enemyCollisions(Enemy enemy[], Rectangle playerRec, int i){

    
}

void enemyDraw(Enemy enemy[]){
    for(int i = 0; i < ENEMY_NUM; i++){
        if(enemy[i].active){
            DrawRectangleRec(enemy[i].rec, RED);

            DrawRectangle(enemy[i].rec.x, enemy[i].rec.y - 50, (int)(enemy[i].rec.width * (enemy[i].health/enemy[i].baseHealth)), 20, BLACK);
        }
    }
}

void enemyDelete(Enemy enemy[], int i){
    enemy[i].active = false;
    enemy[i].dir = (Vector2){0,0};
    enemy[i].pos = (Vector2){0,0};
    enemy[i].rec = (Rectangle){0,0,0,0};
    enemy[i].speed = 0;
    enemy[i].health = -1;
    enemy[i].state = NOT_HIT;
}
