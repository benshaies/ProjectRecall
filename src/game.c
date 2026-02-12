#include "../headers/game.h"
#include "math.h"
#include "../headers/player.h"
#include "../headers/enemy.h"
#include "../headers/textures.h"
#include "raymath.h"
#include "../headers/arena.h"
#include "stdio.h"
#include "stdlib.h"

const int GAME_WIDTH = 1280;
const int GAME_HEIGHT = 720;
RenderTexture2D target;

Game game;

Player player;

Vector2 mousePos;
Vector2 worldMouse;

Enemy enemy[ENEMY_NUM];

bool startGame = false;
bool drawColliders = false;

Camera2D camera;

int screenShake = 0;
int screenShakeFrameBase = 6;
int screenShakeValue = 7;

float hitStopTime = 0.025;
float hitStopTimer = 0;

float enemySpawnTime = 3.5f;

//Level variables
const char *fileName = "../arena/arena_walls.csv";
const char *propsFileName = "../arena/arena_props.csv";
const char *floorFileName = "../arena/arena_floor.csv";


int temp = 0;


void gameInit(){
    InitWindow(GAME_WIDTH, GAME_HEIGHT, "Project Recall");  
    SetTargetFPS(60);

    texturesLoad();

    target = LoadRenderTexture(GAME_WIDTH, GAME_HEIGHT);
    SetTextureFilter(target.texture, TEXTURE_FILTER_POINT); 

    HideCursor();

    //Loads csv values into array
    csvToArray(game.levelArray, fileName);
    csvToArray(game.propsArray, propsFileName);
    csvToArray(game.floorArray, floorFileName);

    //Create collision Recs
    game.colliderCount = getWallAmount(game.levelArray);
    game.colliderRecs = malloc(getWallAmount(game.levelArray) * sizeof(Rectangle));
    createCollisionRecs(game.levelArray, game.colliderRecs);

    game.enemySpawnTimer = 0;

    playerInit(&player);

    camera.offset = (Vector2){GAME_WIDTH/2, GAME_HEIGHT/2};
    camera.target = player.pos;
    camera.zoom = 0.75f;
}

void cameraShake(){
    if(screenShake > 0){
        screenShake--;
        camera.offset = (Vector2){camera.offset.x + GetRandomValue(-screenShakeValue,screenShakeValue), camera.offset.y + GetRandomValue(-screenShakeValue,screenShakeValue)};
    }
    else{
        //camera.offset = (Vector2){GAME_WIDTH/2, GAME_HEIGHT/2};
        camera.offset.x = Lerp(camera.offset.x, GAME_WIDTH/2, 5* GetFrameTime());
        camera.offset.y = Lerp(camera.offset.y, GAME_HEIGHT/2, 5 * GetFrameTime());
    }
}

void updateCamera(){

    Vector2 mouseDir = { worldMouse.x - player.pos.x, worldMouse.y - player.pos.y };
    mouseDir = Vector2Normalize(mouseDir);
    mouseDir = Vector2Scale(mouseDir, 80);

    Vector2 desiredTarget = {
        player.pos.x + mouseDir.x,
        player.pos.y + mouseDir.y
    };

    camera.target.x = (Lerp(camera.target.x, desiredTarget.x, 10 * GetFrameTime()));
    camera.target.y = (Lerp(camera.target.y, desiredTarget.y, 10 * GetFrameTime()));

    cameraShake();

}


void gameSetFullscreen(){
    if(IsKeyPressed(KEY_F11)){
        ToggleBorderlessWindowed();
    }
}



void gameUpdate(){
    gameSetFullscreen();


    if(hitStopTimer <= 0){
        playerUpdate(&player, game.colliderRecs, game.colliderCount);
        updateCamera();

        if(IsKeyPressed(KEY_TAB)){
            startGame = !startGame;
        }
        if(startGame){
            spawnEnemies();
        }
        if(IsKeyPressed(KEY_F1)){
            drawColliders = !drawColliders;
        }
        
        
        //Returns true if hit, starts screenshake
        if(enemyUpdate(enemy, player.rec, player.axe, player.pos)){
            screenShake = screenShakeFrameBase;
            hitStopTimer = hitStopTime;
        }
    }
    else{
        hitStopTimer -= GetFrameTime();
    }
    

}

void spawnEnemies(){
    game.enemySpawnTimer += GetFrameTime();
    if(game.enemySpawnTimer >= enemySpawnTime){
        enemyInit(enemy, player.pos);
        game.enemySpawnTimer = 0;
    }
}

void gameResolutionDraw(){
    BeginDrawing();
        ClearBackground(WHITE);

        float scale = fmin(
            (float)GetScreenWidth()  / GAME_WIDTH,
            (float)GetScreenHeight() / GAME_HEIGHT
        );

        float scaledWidth  = GAME_WIDTH  * scale;
        float scaledHeight = GAME_HEIGHT * scale;

        float offsetX = (GetScreenWidth()  - scaledWidth)  * 0.5f;
        float offsetY = (GetScreenHeight() - scaledHeight) * 0.5f;

        Rectangle src = { 
            0, 0, 
            (float)target.texture.width, 
            -(float)target.texture.height   // flip vertically
        };

        Rectangle dst = {
            offsetX, offsetY,
            scaledWidth, scaledHeight
        };

        DrawTexturePro(target.texture, src, dst, (Vector2){0,0}, 0.0f, WHITE);

        mousePos = GetMousePosition();

        mousePos.x = (mousePos.x - offsetX) / scale;
        mousePos.y = (mousePos.y - offsetY) / scale;
        worldMouse = GetScreenToWorld2D(mousePos, camera);


    EndDrawing();
}

void gameDraw(){
    BeginTextureMode(target);

        ClearBackground(BLACK);

        

        BeginMode2D(camera);
            
            //FLoor
            drawFloor(game.floorArray);
            //Walls
            drawLevel(game.levelArray, 0);
            //Props
            drawLevel(game.propsArray, 1);

            playerDraw(&player);
            enemyDraw(enemy);

            if(drawColliders){
                drawColliderRecs();
            }
            


        EndMode2D();
        
        //Draw cursor

        DrawTexturePro(cursorTexture, (Rectangle){0,0,16,16}, (Rectangle){mousePos.x - 15, mousePos.y - 15, 30, 30}, (Vector2){0,0}, 0.0f, WHITE); 
    EndTextureMode();

    gameResolutionDraw();
}

void drawColliderRecs(){
    for(int i = 0; i < game.colliderCount; i++){
        DrawRectangleLinesEx(game.colliderRecs[i], 2.5f, RED);
    }
}