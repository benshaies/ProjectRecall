#include "../headers/game.h"
#include "math.h"
#include "../headers/player.h"
#include "../headers/enemy.h"
#include "../headers/textures.h"

const int GAME_WIDTH = 1280;
const int GAME_HEIGHT = 720;
RenderTexture2D target;

Game game;

Player player;

Vector2 mousePos;

Enemy enemy[ENEMY_NUM];

bool startGame = false;

void gameInit(){
    InitWindow(GAME_WIDTH, GAME_HEIGHT, "Project Recall");  
    SetTargetFPS(60);

    target = LoadRenderTexture(GAME_WIDTH, GAME_HEIGHT);
    SetTextureFilter(target.texture, TEXTURE_FILTER_POINT); 

    texturesLoad();

    game.enemySpawnTimer = 0;

    playerInit(&player);
    // /enemyInit(enemy, player.pos);
}

void gameSetFullscreen(){
    if(IsKeyPressed(KEY_F11)){
        ToggleBorderlessWindowed();
    }
}

void gameUpdate(){
    gameSetFullscreen();

    playerUpdate(&player);

    if(IsKeyPressed(KEY_TAB)){
        startGame = !startGame;
    }
    if(startGame){
        spawnEnemies();
    }
    

    enemyUpdate(enemy, player.rec, player.axe);

}

void spawnEnemies(){
    game.enemySpawnTimer += GetFrameTime();
    if(game.enemySpawnTimer > 1.5){
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


    EndDrawing();
}

void gameDraw(){
    BeginTextureMode(target);

        ClearBackground(BLACK);

        playerDraw(&player);
        enemyDraw(enemy);

    
    EndTextureMode();

    gameResolutionDraw();
}