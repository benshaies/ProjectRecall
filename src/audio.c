#include "../headers/audio.h"
#include "raylib.h"

Sound walkingSound, upgradeDisplaySound, upgradeUnlockedSound, throwSound,
    recallSound, enemyHitSound, deflectSound, upgradeSelectedSound,
    playerHurtSound;

Music gameplayMusic[3], menuMusic[3];

void audioFileLoad() {
  // SOUND
  walkingSound = LoadSound("../audio/sound/walking.ogg");
  upgradeDisplaySound = LoadSound("../audio/sound/displayCards.wav");
  upgradeUnlockedSound = LoadSound("../audio/sound/upgradeUnlocked.wav");
  throwSound = LoadSound("../audio/sound/throw.wav");
  recallSound = LoadSound("../audio/sound/recall.wav");
  enemyHitSound = LoadSound("../audio/sound/enemyHit.wav");
  deflectSound = LoadSound("../audio/sound/deflect.wav");
  upgradeSelectedSound = LoadSound("../audio/sound/upgradeSelected.wav");
  playerHurtSound = LoadSound("../audio/sound/playerHurt.wav");

  // MUSIC
  gameplayMusic[0] = LoadMusicStream(
      "../audio/music/gameplayMusic/I've Got Your Back! (Full).ogg");
  gameplayMusic[1] = LoadMusicStream(
      "../audio/music/gameplayMusic/If It's a Fight You Want.ogg");
  gameplayMusic[2] =
      LoadMusicStream("../audio/music/gameplayMusic/It's Bossin Time.ogg");
  gameplayMusic[0].looping = false;
  gameplayMusic[1].looping = false;
  gameplayMusic[2].looping = false;

  menuMusic[0] =
      LoadMusicStream("../audio/music/menuMusic/Journey Across the Blue.ogg");
  menuMusic[1] = LoadMusicStream("../audio/music/menuMusic/Returning Home.ogg");
  menuMusic[2] =
      LoadMusicStream("../audio/music/menuMusic/Smooth As Glass.ogg");
  menuMusic[0].looping = false;
  menuMusic[1].looping = false;
  menuMusic[2].looping = false;
}

void audioFileUnload() {

  UnloadSound(walkingSound);
  UnloadSound(upgradeDisplaySound);
  UnloadSound(upgradeUnlockedSound);
  UnloadSound(throwSound);
  UnloadSound(recallSound);
  UnloadSound(enemyHitSound);
  UnloadSound(deflectSound);
  UnloadSound(upgradeSelectedSound);
  UnloadSound(playerHurtSound);

  UnloadMusicStream(gameplayMusic[0]);
  UnloadMusicStream(gameplayMusic[1]);
  UnloadMusicStream(gameplayMusic[2]);

  UnloadMusicStream(menuMusic[0]);
  UnloadMusicStream(menuMusic[1]);
  UnloadMusicStream(menuMusic[2]);

  CloseAudioDevice();
}
