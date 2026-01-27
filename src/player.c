#include "../headers/player.h"
#include "raymath.h"
#include "stdio.h"
#include "../headers/game.h"

int player_width = 75;
int player_height = 75;

const float axeBaseSpeed = 10;
const float axeRecallSpeed = 13;
const float axeSpeedIncrementThrown = 0.25;
const float axeSpeedIncrementRecall = 0.75;

void playerInit(Player *player){
    player->pos = (Vector2){1280/2, 720/2};
    player->rec = (Rectangle){player->pos.x, player->pos.y, player_width, player_height};
    player->speed = 5.0f;
    player->dir = (Vector2){0,0};
    player->state = IDLE;

    //Weapon stuff
    player->axe.pos = (Vector2){player->pos.x, player->pos.y};
    player->axe.rec = (Rectangle){player->axe.pos.x,player->axe.pos.y, 20, 20};
    player->axe.throwSpeed = axeBaseSpeed;
    player->axe.recallSpeed = axeRecallSpeed;
    player->axe.state = HOLDING;
    player->axe.attackPos = (Vector2){0,0};
    player->axe.attackCheckRadius = 25.0;
}

void playerUpdate(Player *player){
    playerMovement(player);

    axeUpdate(player);
}

void playerMovement(Player *player){
    
    //Get input
    if(IsKeyDown(KEY_A)){
        player->dir.x -= 1;
    }
    else if(IsKeyDown(KEY_D)){
        player->dir.x += 1;
    }

    if(IsKeyDown(KEY_W)){
        player->dir.y -= 1;
    }
    else if(IsKeyDown(KEY_S)){
        player->dir.y += 1;
    }

    //Add direction to player but first normalize
    player->dir = Vector2Normalize(player->dir);
    player->pos.x += player->dir.x * player->speed;
    player->pos.y += player->dir.y * player->speed;

    //Reset dir
    player->dir = (Vector2){0,0};


    //Update rec
    player->rec = (Rectangle){player->pos.x, player->pos.y, player_width, player_height};

}

void axeUpdate(Player *player){
    switch (player->axe.state){
        case HOLDING:
            player->axe.pos = player->pos;
            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                player->axe.attackPos = mousePos;
                player->axe.dir = (Vector2){player->axe.attackPos.x - player->axe.rec.x, player->axe.attackPos.y - player->axe.rec.y};
                player->axe.dir = Vector2Normalize(player->axe.dir); 
                player->axe.state = THROWN;
            }

            break;

        case THROWN:

            player->axe.pos.x += player->axe.dir.x * player->axe.throwSpeed;
            player->axe.pos.y += player->axe.dir.y * player->axe.throwSpeed;

            player->axe.throwSpeed += axeSpeedIncrementThrown;

            if(CheckCollisionCircleRec(player->axe.attackPos, player->axe.attackCheckRadius, player->axe.rec)){
                player->axe.attackPos = (Vector2){0,0};
                player->axe.dir = (Vector2){0,0};
                player->axe.state = DONE_THROW;

                player->axe.throwSpeed = axeBaseSpeed;
            }
            break;
            

        case DONE_THROW:
            if(IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)){
                player->axe.dir = (Vector2){player->pos.x - player->axe.pos.x, player->pos.y - player->axe.pos.y};
                player->axe.state = RECALL;
            }
            break;

        case RECALL:
            if(!CheckCollisionRecs(player->rec, player->axe.rec)){
                player->axe.dir = (Vector2){player->pos.x - player->axe.pos.x, player->pos.y - player->axe.pos.y};
                player->axe.dir = Vector2Normalize(player->axe.dir); 
                player->axe.pos.x += player->axe.dir.x * player->axe.recallSpeed;
                player->axe.pos.y += player->axe.dir.y * player->axe.recallSpeed;   
                player->axe.recallSpeed += axeSpeedIncrementRecall;
            }
            else{
                player->axe.state = HOLDING;
                player->axe.recallSpeed = axeRecallSpeed;
            }
            break;
            
        
        

    }

    //Update axe rec
    player->axe.rec = (Rectangle){player->axe.pos.x, player->axe.pos.y, 25, 25};
    printf("x:%f y:%f \n", player->axe.pos.x, player->axe.pos.y);
}

void playerDraw(Player *player){
    DrawRectangleRec(player->rec, BLACK);
    DrawRectangleRec(player->axe.rec, BLUE);

    if(player->axe.state == THROWN){
        DrawCircle(player->axe.attackPos.x, player->axe.attackPos.y, player->axe.attackCheckRadius, RED);
    }
}