#ifndef TEXTURES_H
#define TEXTURES_H
#include "raylib.h"

extern Texture2D playerIdleTexture, playerSideTexture, levelTilesetTexture, enemyIdleTexture, enemyHitTexture, cursorTexture;




void texturesLoad();

void texturesUnload();

#endif