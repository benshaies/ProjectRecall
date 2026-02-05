#include "../headers/textures.h"

Texture2D playerIdleTexture, playerSideTexture, levelTilesetTexture;

void texturesLoad(){
    playerIdleTexture = LoadTexture("../assets/playerIdle.png");
    playerSideTexture = LoadTexture("../assets/playerSide.png");
    levelTilesetTexture = LoadTexture("../assets/tempLevelTileset.png");

    SetTextureFilter(levelTilesetTexture, TEXTURE_FILTER_POINT);

}

void texturesUnload(){
    UnloadTexture(playerIdleTexture);
    UnloadTexture(playerSideTexture);
    UnloadTexture(levelTilesetTexture);
}