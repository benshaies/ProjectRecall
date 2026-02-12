#include "../headers/textures.h"

Texture2D playerIdleTexture, playerSideTexture, levelTilesetTexture;
Texture2D enemyIdleTexture, enemyHitTexture;
Texture2D cursorTexture;
Texture2D axeThrowTexture;

void texturesLoad(){
    playerIdleTexture = LoadTexture("../assets/playerIdle.png");
    playerSideTexture = LoadTexture("../assets/playerSide.png");
    levelTilesetTexture = LoadTexture("../assets/levelTileset.png");

    enemyIdleTexture = LoadTexture("../assets/enemyIdle.png");
    enemyHitTexture = LoadTexture("../assets/enemyHit.png");

    cursorTexture = LoadTexture("../assets/cursor.png");

    axeThrowTexture = LoadTexture("../assets/axeThrow.png");

    SetTextureFilter(levelTilesetTexture, TEXTURE_FILTER_POINT);

}

void texturesUnload(){
    UnloadTexture(playerIdleTexture);
    UnloadTexture(playerSideTexture);
    UnloadTexture(levelTilesetTexture);

    UnloadTexture(enemyIdleTexture);
    UnloadTexture(enemyHitTexture);

    UnloadTexture(cursorTexture);

    UnloadTexture(axeThrowTexture);
}