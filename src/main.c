#include "../headers/audio.h"
#include "../headers/game.h"
#include "../headers/textures.h"
#include "raylib.h"

int main(void) {

  gameInit();
  ToggleBorderlessWindowed();

  while (!WindowShouldClose()) {
    gameUpdate();
    gameDraw();
  }
  texturesUnload();
  audioFileUnload();
  CloseWindow();
  return 0;
}
