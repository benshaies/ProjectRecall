#ifndef TRANSITION_H
#define TRANSITION_H
#include "game.h"
#include <raylib.h>

typedef struct {

  int ringDistance;
  bool transitionIn;
  bool transitonOut;
  bool transitionDone;
} Transition;

void resetTransition();

void createTransition(GameState *state, GameState newState);

void drawTransition();

#endif // !d
