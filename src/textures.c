#include "../headers/textures.h"

Texture2D playerIdleTexture, playerSideTexture;

void texturesLoad(){
    playerIdleTexture = LoadTexture("../assets/playerIdle.png");
    playerSideTexture = LoadTexture("../assets/playerSide.png");
}

void texturesUnload(){
    UnloadTexture(playerIdleTexture);
    UnloadTexture(playerSideTexture);
}