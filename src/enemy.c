#include "../headers/enemy.h"
#include "raymath.h"
#include "stdio.h"

void enemyInit(Enemy enemy[], Vector2 playerPos){
    for (int i = 0; i < ENEMY_NUM; i++){
        if(!enemy[i].active){
            enemy[i].pos = (Vector2){GetRandomValue(0, 1280), GetRandomValue(0, 720)};
            enemy[i].rec = (Rectangle){enemy[i].pos.x, enemy[i].pos.y, 50, 50};
            enemy[i].speed = 2.0f;
            enemy[i].dir = Vector2Normalize((Vector2){playerPos.x - enemy[i].pos.x, playerPos.y - enemy[i].pos.y});
            enemy[i].active = true;
            enemy[i].health = 100;
            enemy[i].baseHealth = 100.0f;
            enemy[i].state = NOT_HIT;
            enemy[i].knockbackDir = (Vector2){0,0};
            enemy[i].color = GREEN;
            enemy[i].hitFrameCount = 0;
            break;
        }
    }
}

void enemyUpdate(Enemy enemy[], Rectangle playerRec, Weapon axe){
    for(int i = 0; i < ENEMY_NUM; i++){
        if(enemy[i].active){

            

            enemy[i].dir = Vector2Normalize((Vector2){playerRec.x - enemy[i].pos.x, playerRec.y - enemy[i].pos.y});
            enemy[i].pos.x += enemy[i].dir.x * enemy[i].speed;
            enemy[i].pos.y += enemy[i].dir.y * enemy[i].speed;
            enemy[i].rec = (Rectangle){enemy[i].pos.x, enemy[i].pos.y, 50, 50};
        

            //Enemy hit check (Only acitvates if enemy isnt currently hit) 
            //1 = THROWN
            //2 = RECALL
            if(CheckCollisionRecs(enemy[i].rec, axe.rec) && enemy[i].state == NOT_HIT && (axe.state == 1 || axe.state == 3)) {
                enemy[i].knockbackDir = Vector2Normalize(axe.dir);

                enemy[i].color = WHITE;
                enemy[i].health -= axe.damage;
                enemy[i].state = HIT;

            }

            //Add knockback to enemy
            if(enemy[i].state == HIT){
                enemy[i].pos.x += enemy[i].knockbackDir.x * axe.throwSpeed;
                enemy[i].pos.y += enemy[i].knockbackDir.y * axe.throwSpeed;

                enemy[i].hitFrameCount++;

                if(enemy[i].hitFrameCount >= 4){
                    enemy[i].state = IMMUNE;
                    enemy[i].hitFrameCount = 0;
                }
                

            }

            //Enemy is hit but immune now
            if(enemy[i].state == IMMUNE){
                enemy[i].color = RED;
                if(!CheckCollisionRecs(enemy[i].rec, axe.rec)){
                    enemy[i].state = NOT_HIT;
                }
            }

            //If axe and enemy not colliding, resets state
            if(!CheckCollisionRecs(enemy[i].rec,axe.rec) && enemy[i].state != IMMUNE){
                enemy[i].state = NOT_HIT;
                enemy[i].color = GREEN;
            }

            //Remove enemy once health is 0
            if(enemy[i].health <= 0){
                enemyDelete(enemy, i);
            }


            //Check Collision with player
            if(CheckCollisionRecs(enemy[i].rec, playerRec)){
                //CloseWindow();
            }

        
        }
    }
}

void enemyCollisions(Enemy enemy[], Rectangle playerRec, int i){

    
}

void enemyDraw(Enemy enemy[]){
    for(int i = 0; i < ENEMY_NUM; i++){
        if(enemy[i].active){
            DrawRectangleRec(enemy[i].rec, enemy[i].color);

            DrawRectangle(enemy[i].rec.x, enemy[i].rec.y - 50, (int)(enemy[i].rec.width * (enemy[i].health/enemy[i].baseHealth)), 20, WHITE);
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
