#include "raylib.h"
#include "../headers/game.h"


int main(void){

    gameInit();
    ToggleBorderlessWindowed();
    while (!WindowShouldClose()){
        gameUpdate();
        gameDraw();
    }

    CloseWindow();
    return 0;
}