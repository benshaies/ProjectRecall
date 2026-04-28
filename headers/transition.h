#ifndef TRANSITION_H
#define TRANSITION_H
#include "game.h"
#include <raylib.h>

typedef enum {
  NOT_ALIVE,
  TRANS_IN,
  SWITCH_GAMESTATE,
  TRANS_OUT,
  TRANS_DONE,
} TransitionState;

typedef enum {
  TO_MENU,
  TO_PLAYING,
} TransitionType;

typedef struct {

  float ringDistance;

  TransitionState state;
  TransitionType type;
  float transPauseTimer;
  float transPauseTimeBase;

  Sound sound;
  bool soundPlayed;
} Transition;

void transitionSet(Transition *t, Sound sound);

void transitionInit(Transition *t, TransitionType type, float pauseTimer);

void transitionUpdate(Transition *t, GameState *state);

void transitionDraw(Transition *t);

#endif // !d
