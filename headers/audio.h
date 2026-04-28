#ifndef AUDIO_H
#define AUDIO_H
#include "raylib.h"

extern Sound walkingSound, upgradeDisplaySound, upgradeUnlockedSound,
    throwSound, recallSound, enemyHitSound, deflectSound, upgradeSelectedSound,
    playerHurtSound, upgradeCardHoverSound, gameOverSound,
    gameOverScreenFallSound;

extern Sound otherStatsDisplaySound, scoreDisplaySound,
    newHighScoreDisplaySound, transitionSound, selectSound;

extern Music gameplayMusic[3], menuMusic[3], idleMusic;

void audioFileLoad();

void audioFileUnload();

#endif // !AUDIO_H
