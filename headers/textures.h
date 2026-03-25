#ifndef TEXTURES_H
#define TEXTURES_H
#include "raylib.h"
#include "upgrade.h"

extern Texture2D playerIdleTexture, playerSideTexture, levelTilesetTexture, enemyIdleTexture, enemyHitTexture, cursorTexture, axeThrowTexture, axeBaseTexture;
extern Texture2D heartTexture, heartHalfTexture, playerHurtTexture, heartEmptyTexture, enemy2RunTexture, enemy2HitTexture, enemyOneAttackTexture;
extern Texture2D upgradeScreenBaseTexture;
extern Texture2D upgradeTextures[NUMBER_OF_UPGRADES];

void texturesLoad();

void texturesUnload();

#endif