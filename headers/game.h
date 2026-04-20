#ifndef GAME_H
#define GAME_H
#include "arena.h"
#include "raylib.h"

extern Vector2 mousePos;
extern Vector2 worldMouse;

typedef enum {
  MAIN,
  GUIDE,
} MenuState;

typedef enum {
  MAIN_MENU,
  PLAYING,
  DYING_TRANSITION,
  DEAD,
  UPGRADE_SCREEN,
  TESTING,
  PAUSED,
} GameState;

typedef struct {
  float timer;
  float delay;
  bool triggered;
  bool particleTriggered;
} TimedEvent;

typedef struct {
  float enemySpawnTimer;
  int levelArray[LEVEL_HEIGHT][LEVEL_WIDTH];
  int propsArray[LEVEL_HEIGHT][LEVEL_WIDTH];
  int floorArray[FlOOR_HEIGHT][FLOOR_WIDTH];
  Rectangle *colliderRecs;
  int colliderCount;

  GameState state;

  int score;
  int scoreThresholdNum;

  int enemiesKilled;
  float timeSurvived;

  float masterVolume;

  MenuState menuState;
} Game;

void gameInit();

void gameSetFullscreen();

void gameUpdate();
void resetTimedEvent(TimedEvent *event, float delay);

bool updateTimedEvent(TimedEvent *event);

void gameResolutionDraw();

void gameDraw();

void spawnEnemies();

void resetGame();
#endif
