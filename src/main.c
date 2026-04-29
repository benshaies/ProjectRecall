#include "../headers/audio.h"
#include "../headers/game.h"
#include "../headers/textures.h"
#include "raylib.h"
#include <stdlib.h>

int main(void) {

  gameInit();
  ToggleBorderlessWindowed();

  while (!WindowShouldClose()) {
    if (gameUpdate() == 1) {
      break;
    }
    gameDraw();
  }
  texturesUnload();
  audioFileUnload();
  CloseWindow();
  return 0;
}
