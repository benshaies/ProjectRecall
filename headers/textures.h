#ifndef TEXTURES_H
#define TEXTURES_H
#include "raylib.h"

extern Texture2D playerIdleTexture, playerSideTexture, levelTilesetTexture, enemyIdleTexture, enemyHitTexture, cursorTexture, axeThrowTexture, axeBaseTexture;




void texturesLoad();

void texturesUnload();

#endif