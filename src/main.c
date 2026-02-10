#include "raylib.h"
#include "../headers/game.h"
#include "../headers/textures.h"


int main(void){

    gameInit();
    //ToggleBorderlessWindowed();
    while (!WindowShouldClose()){
        gameUpdate();
        gameDraw();
    }
    texturesUnload();

    CloseWindow();
    return 0;
}