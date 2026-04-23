#include "../headers/transition.h"
#include "raylib.h"

Transition transition;

void resetTransition() {
  transition.transitionIn = false;
  transition.transitonOut = false;
  transition.transitionDone = false;
  transition.ringDistance = 1280;
}

void createTransition(GameState *state, GameState newState) {

  if (!transition.transitionDone) {
    if (transition.ringDistance > 0 && !transition.transitonOut) {
      transition.ringDistance -= 20;
      transition.transitionIn = true;
    } else {
      *state = newState;
      transition.transitionIn = false;
      transition.transitonOut = true;
    }

    if (transition.transitonOut) {
      transition.ringDistance -= 20;
      if (transition.ringDistance >= 1280) {
        transition.ringDistance = 1280;
        transition.transitionDone = true;
      }
    }
  }
}

void drawTransition() {
  DrawRing((Vector2){1280 / 2, 720 / 2}, transition.ringDistance, 10000, 0, 360,
           1, BLACK);
}
