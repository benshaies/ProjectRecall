#include "../headers/transition.h"
#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>

void transitionSet(Transition *t, Sound sound) {
  t->state = NOT_ALIVE;
  t->sound = sound;
}

void transitionInit(Transition *t, TransitionType type, float pauseTimer) {

  if (t->state == NOT_ALIVE) {
    t->state = TRANS_IN;
    t->type = type;

    t->ringDistance = 1280;
    t->transPauseTimer = pauseTimer;
    t->transPauseTimeBase = pauseTimer;
    t->soundPlayed = false;
  }
}

void transitionUpdate(Transition *t, GameState *state) {

  switch (t->state) {
  case TRANS_IN:

    if (!t->soundPlayed) {
      SetSoundPitch(t->sound, GetRandomValue(3, 5) * 0.2);
      PlaySound(t->sound);
      t->soundPlayed = true;
    }

    if (t->ringDistance > 0) {
      t->ringDistance -= 20;
    } else {
      t->ringDistance = 0;
      t->state = SWITCH_GAMESTATE;
    }

    break;
  case SWITCH_GAMESTATE:
    *state = TRANSITION;
    resetGame();
    t->soundPlayed = false;
    t->transPauseTimer -= GetFrameTime();

    if (t->transPauseTimer <= 0) {
      if (t->type == TO_MENU) {
        *state = MAIN_MENU;
      } else if (t->type == TO_PLAYING) {
        *state = PLAYING;
      }

      t->state = TRANS_OUT;
      t->transPauseTimer = t->transPauseTimeBase;
    }
    break;

  case TRANS_OUT:
    if (!t->soundPlayed) {
      SetSoundPitch(t->sound, GetRandomValue(3, 5) * 0.2);
      PlaySound(t->sound);
      t->soundPlayed = true;
    }

    if (t->ringDistance < 1280) {
      t->ringDistance += 20;
    } else {
      t->ringDistance = 1280;
      t->state = TRANS_DONE;
    }

    break;

  case TRANS_DONE:
    t->state = NOT_ALIVE;
    break;
  }
}

void transitionDraw(Transition *t) {
  DrawRing((Vector2){1280 / 2, 720 / 2}, t->ringDistance, 100000, 0, 360, 1,
           BLACK);
}
