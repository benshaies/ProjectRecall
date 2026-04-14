#include "../headers/audio.h"
#include "raylib.h"

Sound walkingSound, upgradeDisplaySound;

Music gameplayMusic[3], menuMusic[3];

void audioFileLoad() {
  // SOUND
  walkingSound = LoadSound("../audio/sound/walking.ogg");
  upgradeDisplaySound = LoadSound("../audio/sound/displayCards.wav");

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

  // Setting volume
  SetSoundVolume(walkingSound, 0.6);

  SetMusicVolume(gameplayMusic[0], 0.1);
  SetMusicVolume(gameplayMusic[1], 0.1);
  SetMusicVolume(gameplayMusic[2], 0.1);
}

void audioFileUnload() {

  UnloadSound(walkingSound);
  UnloadSound(upgradeDisplaySound);

  UnloadMusicStream(gameplayMusic[0]);
  UnloadMusicStream(gameplayMusic[1]);
  UnloadMusicStream(gameplayMusic[2]);

  UnloadMusicStream(menuMusic[0]);
  UnloadMusicStream(menuMusic[1]);
  UnloadMusicStream(menuMusic[2]);

  CloseAudioDevice();
}
