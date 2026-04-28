#include "../headers/game.h"
#include "../headers/arena.h"
#include "../headers/audio.h"
#include "../headers/enemy.h"
#include "../headers/particles.h"
#include "../headers/player.h"
#include "../headers/textures.h"
#include "../headers/upgrade.h"
#include "math.h"
#include "raylib.h"
#include "raymath.h"
#include "stdio.h"
#include <math.h>
#define RAYGUI_IMPLEMENTATION
#include "../headers/transition.h"
#include "stdlib.h"
#include <raygui.h>
#include <string.h>

const int GAME_WIDTH = 1280;
const int GAME_HEIGHT = 720;
RenderTexture2D target;

Game game;

Player player;

Vector2 mousePos;
Vector2 worldMouse;

Enemy enemy[ENEMY_NUM];

bool debugMode = false;

Camera2D camera;

// Normal screenshake and hitstop stuff
int screenShake = 0;
int screenShakeFrameBase = 6;
int screenShakeValue = 7;

float hitStopTime = 0.025;
float hitStopTimer = 0;

// Throwing boomerang screenshake
int boomerangThrownScreenShake = 0;

// Check for if player hit screenshake has happened yet
bool playerScreenShakeStarted = false;
float hitStopTimePlayer = 1.0f;

float playerHitStopTime = 0.025f;

float enemySpawnTime = 2.0f;

Rectangle enemyAttackRec;

// Level variables
const char *fileName = "../arena/arena_walls.csv";
const char *propsFileName = "../arena/arena_props.csv";
const char *floorFileName = "../arena/arena_floor.csv";

int temp = 0;

// Game wave system varaibles
bool startGame = false;

// Health rectangles
Rectangle healthRecs[3];

// Particle variables
ParticleSystem ps;

int playerRunParticleCheck = 0;
Color playerTrailColor = {33, 19, 25, 255};

int boomerangThrowParticleCheck = 0;
Color boomerangTrailColor = {18, 78, 137, 255};
Color boomerangTrailColor2 = {44, 232, 245, 255};

Color enemyHitParticleColor = {162, 38, 51, 255};
bool playerHitParticleStarted = false;

// Gameplay specific variables
bool isBoomerangDeflected = false;

// Upgrade screen stuff
Rectangle baseRec = {290, 160, 700, 400};
UpgradeScreen upgradeScreen;
bool startUpgrades = false;

// Gameover screen variables
Rectangle gameOverRec = {390, -400, 500, 400};
float gameOverRecVelY = 0.0f;
bool hasFallen = false;
float gameOverRecGravity = 1.5f;
int bounceCount = 0;

Font font;

TimedEvent displayTimeSurvied;
float displayTimeSurvivedTimer = 0.75f;

TimedEvent displayEnemiesKilled;
float displayEnemiesKilledTimer = 0.75f;

TimedEvent displayScore;
float displayScoreTimer = 1.25f;

Rectangle quitButtonRec = {440, 460, 150, 60};
Rectangle quitButtonRecBase = {440, 460, 150, 60};

Color quitButtonColor = (Color){24, 20, 37, 255};
Color secondaryColor = (Color){115, 62, 57, 255};
Color playAgainButtonColor = (Color){24, 20, 57, 255};
Color buttonColor = (Color){24, 20, 37, 255};
Color hightlightColor = (Color){115, 62, 57, 255};

Rectangle playAgainButtonRec = {690, 460, 150, 60};
Rectangle playAgainButtonRecBase = {690, 460, 150, 60};

bool gameOverAnimationsDone = false;

bool isGameOverSoundPlayed = false;
bool isNewHighScore = false;
int newHighScoreFrameCount = 0;
float newHighScoreDisplayTimer = 0.0f;

// Paused screen variables

bool pausedScreenOpen = false;
float pauseScreenOpeningProgress = 0.0f;
Rectangle pausedScreenRec = {390, 160, 500, 400};
bool playGameplayMusic = true;
Rectangle playGameplayMusicRec = {390 + 50, 160 + 200, 25, 25};

Rectangle quitToMainMenuButtonRec = {560, 460, 150, 60};

// Playing to gamepver transition variable
float playDeadAnimationTimer = 0;

// Sound stuff
int currentGameplaySong;
bool gameplayMusicStarted = false;

int currentMenuSong = 0;
bool menuMusicStarted = false;

// Menu variables
Vector2 startTextPos = {390, 325};
Vector2 guideTextPos = {400, 400};
int textBobbingSpeed = 25;

Rectangle menuBoomerangRec = {390, 500, 75, 75};
int menuBoomerangSpeed = 750;

// Transition variables
float ringDistance = 1280;

Transition trans;

void SaveHighScore(int score) {
  FILE *f = fopen(SAVE_FILE, "wb");
  if (!f)
    return;
  SaveData data = {score};
  fwrite(&data, sizeof(SaveData), 1, f);
  fclose(f);
}

int LoadHighScore(void) {
  FILE *f = fopen(SAVE_FILE, "rb");
  if (!f)
    return 0; // no save file yet, default to 0
  SaveData data = {0};
  fread(&data, sizeof(SaveData), 1, f);
  fclose(f);
  return data.highScore;
}

void gameInit() {
  InitWindow(GAME_WIDTH, GAME_HEIGHT, "Project Recall");
  SetTargetFPS(60);

  SetExitKey(KEY_NULL);

  InitAudioDevice();
  audioFileLoad();

  font = LoadFontEx("../PixeloidSans-Bold.ttf", 64, 0, 0);

  game.state = MAIN_MENU;
  game.menuState = MAIN;
  memset(&ps, 0, sizeof(ParticleSystem));

  texturesLoad();

  target = LoadRenderTexture(GAME_WIDTH, GAME_HEIGHT);
  SetTextureFilter(target.texture, TEXTURE_FILTER_POINT);

  HideCursor();

  game.startGame = false;
  game.startGameTimer = 3.0f;

  // Loads csv values into array
  csvToArray(game.levelArray, fileName);
  csvToArray(game.propsArray, propsFileName);
  csvToArray(game.floorArray, floorFileName);

  // Init health recs
  healthRecs[0] = (Rectangle){10, 5, 40, 40};
  healthRecs[1] = (Rectangle){55, 5, 40, 40};
  healthRecs[2] = (Rectangle){95, 5, 40, 40};

  // Create collision Recs
  game.colliderCount = getWallAmount(game.levelArray);
  game.colliderRecs =
      malloc(getWallAmount(game.levelArray) * sizeof(Rectangle));
  createCollisionRecs(game.levelArray, game.colliderRecs);

  game.enemySpawnTimer = 0;

  playerInit(&player, false);

  camera.offset = (Vector2){GAME_WIDTH / 2, GAME_HEIGHT / 2};
  camera.target = player.pos;
  camera.zoom = 0.75f;

  game.score = 0;
  game.scoreThresholdNum = 1;
  game.highScore = LoadHighScore();

  game.enemiesKilled = 0;
  game.timeSurvived = 0.0f;

  upgradeStructInit(&upgradeScreen);

  // Define Reset timed event stuff
  resetTimedEvent(&displayTimeSurvied, displayTimeSurvivedTimer,
                  otherStatsDisplaySound);
  resetTimedEvent(&displayEnemiesKilled, displayEnemiesKilledTimer,
                  otherStatsDisplaySound);
  resetTimedEvent(&displayScore, displayScoreTimer, scoreDisplaySound);

  game.masterVolume = 0.5f;

  trans.ringDistance = 1280;
  transitionSet(&trans, transitionSound);

  PlayMusicStream(idleMusic);
}

// SAVE FILE FUNCTIONS

void resetGame() {
  playerInit(&player, true);

  for (int i = 0; i < ENEMY_NUM; i++) {
    enemyDelete(enemy, i);
  }

  game.startGame = false;

  quitButtonColor = buttonColor;

  // Reset game function
  game.score = 0;
  game.enemiesKilled = 0;
  game.timeSurvived = 0.0f;
  game.scoreThresholdNum = 1;
  // Reset game over stuff
  gameOverRec = (Rectangle){390, -400, 500, 400};
  gameOverRecVelY = 0.0f;
  hasFallen = false;
  gameOverRecGravity = 1.5f;
  bounceCount = 0;
  gameOverAnimationsDone = false;
  playDeadAnimationTimer = 0.0f;

  resetTimedEvent(&displayTimeSurvied, displayTimeSurvivedTimer,
                  otherStatsDisplaySound);
  resetTimedEvent(&displayEnemiesKilled, displayEnemiesKilledTimer,
                  otherStatsDisplaySound);
  resetTimedEvent(&displayScore, displayScoreTimer, scoreDisplaySound);

  isNewHighScore = false;
}

bool isHovering(Rectangle rec) {
  if (CheckCollisionPointRec(mousePos, rec)) {
    return true;
  } else {
    return false;
  }
}

void spawnEnemies() {
  game.enemySpawnTimer += GetFrameTime();
  if (game.enemySpawnTimer >= enemySpawnTime) {

    // 25 percent chang of spawning shield enemy
    int random = GetRandomValue(1, 4);
    if (random == 4) {
      enemyInit(enemy, player.pos, 2, game.scoreThresholdNum);
    } else {
      enemyInit(enemy, player.pos, 1, game.scoreThresholdNum);
    }
    game.enemySpawnTimer = 0;
  }
}

int checkEnemyAttack() {
  int returnValue = -1;
  for (int i = 0; i < ENEMY_NUM; i++) {
    if (enemy[i].active && enemy[i].isAttacking) {
      returnValue = i;
    }
  }
  return returnValue;
}

void updateScore() {
  game.timeSurvived += GetFrameTime();

  game.score = (game.timeSurvived * 0.25) + (game.enemiesKilled * 10);
}

void manageParticles() {
  // Spawn in player particles
  if (player.animState == RUNNING) {
    Vector2 trailStartPos;

    if (player.animationDir == -1)
      trailStartPos = (Vector2){
          player.pos.x + player.rec.width / 2 - 10 + GetRandomValue(-5, 5),
          player.pos.y + player.rec.height / 2 - 10 + GetRandomValue(-5, 5)};
    else
      trailStartPos = (Vector2){
          player.pos.x - player.rec.width / 2 + 10 + GetRandomValue(-5, 5),
          player.pos.y + player.rec.height / 2 - 5 + GetRandomValue(-5, 5)};

    // Only activates every other 3rd frame
    if (playerRunParticleCheck == 2) {
      spawnParticles(&ps, trailStartPos, GetRandomValue(1, 5) * 0.05,
                     playerTrailColor,
                     (Vector2){-player.dir.x * GetRandomValue(1, 2),
                               GetRandomValue(-1, 1)},
                     5);
      playerRunParticleCheck = 0;
    } else
      playerRunParticleCheck++;
  }

  // Player hit particle effects
  if (player.state == HURT && !playerHitParticleStarted) {
    for (int i = 0; i < 6; i++) {
      spawnParticles(&ps, player.pos, 1.0, enemyHitParticleColor,
                     (Vector2){GetRandomValue(-5, 5), GetRandomValue(-5, 5)},
                     GetRandomValue(10, 15));
    }
    playerHitParticleStarted = true;
  }
  if (player.state == NOTHING) {
    playerHitParticleStarted = false;
  }

  if (player.state == PULLING_IN) {
    spawnParticles(&ps, player.pos, GetRandomValue(1, 5) * 0.01, WHITE,
                   (Vector2){GetRandomValue(-2, 2), GetRandomValue(-2, 2)},
                   GetRandomValue(1, 10));
    spawnParticles(&ps, player.pos, GetRandomValue(1, 5) * 0.025, WHITE,
                   (Vector2){GetRandomValue(-2, 2), GetRandomValue(-2, 2)},
                   GetRandomValue(1, 10));
  }

  // Boomerang particles
  if ((player.axe.state == THROWN || player.axe.state == RECALL)) {
    spawnParticles(&ps, player.axe.pos, GetRandomValue(1, 3) * 0.15,
                   boomerangTrailColor,
                   (Vector2){GetRandomValue(1, 3), GetRandomValue(0, 2)},
                   GetRandomValue(3 + player.upgradeLevels[BIGGER_WEAPON] * 2,
                                  6 + player.upgradeLevels[BIGGER_WEAPON] * 4));
    spawnParticles(&ps, player.axe.pos, GetRandomValue(1, 3) * 0.15,
                   boomerangTrailColor2,
                   (Vector2){GetRandomValue(1, 3), GetRandomValue(0, 2)},
                   GetRandomValue(3 + player.upgradeLevels[BIGGER_WEAPON] * 2,
                                  6 + player.upgradeLevels[BIGGER_WEAPON] * 4));
  }

  // Enemy particles when hit
  for (int i = 0; i < ENEMY_NUM; i++) {

    if (!enemy[i].active)
      continue;
    ;
    if (enemy[i].state != HIT)
      continue;

    spawnParticles(&ps, enemy[i].pos, GetRandomValue(1, 3) * 0.15,
                   enemyHitParticleColor,
                   (Vector2){GetRandomValue(-10, 10), GetRandomValue(-10, 10)},
                   GetRandomValue(10, 20));
    spawnParticles(&ps, enemy[i].pos, GetRandomValue(1, 3) * 0.15,
                   enemyHitParticleColor,
                   (Vector2){GetRandomValue(-10, 10), GetRandomValue(-10, 10)},
                   GetRandomValue(10, 20));
    spawnParticles(&ps, enemy[i].pos, GetRandomValue(1, 3) * 0.15,
                   enemyHitParticleColor,
                   (Vector2){GetRandomValue(-10, 10), GetRandomValue(-10, 10)},
                   GetRandomValue(10, 20));
  }

  updateParticles(&ps);
}

// MUSIC FUNCTIONS
void updateGameplayMusic() {
  if (!gameplayMusicStarted) {
    currentGameplaySong = GetRandomValue(0, 2);
    PlayMusicStream(gameplayMusic[currentGameplaySong]);
    gameplayMusicStarted = true;
  }

  UpdateMusicStream(gameplayMusic[currentGameplaySong]);

  // Check if current song is done
  if (!IsMusicStreamPlaying(gameplayMusic[currentGameplaySong])) {
    int next = GetRandomValue(0, 2);

    while (next == currentGameplaySong)
      next = GetRandomValue(0, 2);

    currentGameplaySong = next;
    PlayMusicStream(gameplayMusic[currentGameplaySong]);
  }
}

void updateMenuMusic() {
  if (!menuMusicStarted) {
    currentMenuSong = GetRandomValue(0, 2);
    PlayMusicStream(menuMusic[currentMenuSong]);
    menuMusicStarted = true;
  }

  UpdateMusicStream(menuMusic[currentMenuSong]);

  if (!IsMusicStreamPlaying(menuMusic[currentMenuSong])) {
    int next = GetRandomValue(0, 2);

    while (next == currentMenuSong)
      next = GetRandomValue(0, 2);

    currentMenuSong = next;
    PlayMusicStream(menuMusic[currentMenuSong]);
  }
}

void updateMusicVolume() {
  float mv = game.masterVolume;

  SetSoundVolume(walkingSound, 0.25 * mv);
  SetSoundVolume(upgradeDisplaySound, 0.7 * mv);
  SetSoundVolume(upgradeUnlockedSound, 0.7 * mv);
  SetSoundVolume(recallSound, 0.40 * mv);
  SetSoundVolume(throwSound, 0.50 * mv);
  SetSoundVolume(enemyHitSound, 2.0 * mv);
  SetSoundVolume(deflectSound, 0.25 * mv);
  SetSoundVolume(upgradeSelectedSound, 0.3 * mv);
  SetSoundVolume(playerHurtSound, 0.7 * mv);
  SetSoundVolume(upgradeCardHoverSound, 0.5 * mv);
  SetSoundVolume(gameOverSound, 0.5 * mv);
  SetSoundVolume(gameOverScreenFallSound, 0.5 * mv);
  SetSoundVolume(otherStatsDisplaySound, 0.5 * mv);
  SetSoundVolume(scoreDisplaySound, 0.5 * mv);
  SetSoundVolume(newHighScoreDisplaySound, 0.5 * mv);
  SetSoundVolume(trans.sound, 0.2 * mv);
  SetSoundVolume(selectSound, 0.25 * mv);

  SetMusicVolume(gameplayMusic[0], 0.10 * mv);
  SetMusicVolume(gameplayMusic[1], 0.10 * mv);
  SetMusicVolume(gameplayMusic[2], 0.10 * mv);

  SetMusicVolume(menuMusic[0], 0.20);
  SetMusicVolume(menuMusic[1], 0.20);
  SetMusicVolume(menuMusic[2], 0.20);
}

// UPGRADE SCREEN DRAW
void gameUpgradeScreenDraw() { drawUpgrades(&upgradeScreen, font); }

// PAUSED STATE FUNCTIONS
void gameUpdatePausedScreen() {
  if (!pausedScreenOpen) {
    pauseScreenOpeningProgress += GetFrameTime() * 10;
    if (pauseScreenOpeningProgress >= 1.0f) {
      pauseScreenOpeningProgress = 1.0f;
      pausedScreenOpen = true;
    }
  }

  if (pausedScreenOpen) {
    if (isHovering(playGameplayMusicRec) &&
        IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
      playGameplayMusic = !playGameplayMusic;
    }

    if (isHovering(quitToMainMenuButtonRec) &&
        IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
      SetSoundPitch(selectSound, GetRandomValue(3, 5) * 0.2);
      PlaySound(selectSound);
      transitionInit(&trans, TO_MENU, 0.25);
    }
  }
}

void DrawVolumeSlider(Vector2 mousePos, float *masterVolume) {
  Rectangle track = {pausedScreenRec.x + 50, pausedScreenRec.y + 150,
                     pausedScreenRec.width - 100, 25};

  // Handle
  float handleX = track.x + (*masterVolume * track.width);
  Rectangle handle = {handleX - 8, track.y - 4, 16, track.height + 8};

  // Input
  if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) &&
      CheckCollisionPointRec(mousePos, track)) {
    *masterVolume = (mousePos.x - track.x) / track.width;
    *masterVolume = Clamp(*masterVolume, 0.0f, 1.0f);
  }

  // Draw
  DrawRectangleRec(track, GRAY);
  DrawRectangle(track.x, track.y, handleX - track.x, track.height,
                quitButtonColor);
  DrawRectangleRec(handle, WHITE);

  // Labels
  DrawTextEx(font, TextFormat("%d%%", (int)(*masterVolume * 100)),
             (Vector2){track.x + track.width + 8, track.y + 4}, 14, 3, WHITE);
}

void gameDrawPausedScreen() {
  DrawTexturePro(gameOverTexture, (Rectangle){0, 0, 80, 64}, pausedScreenRec,
                 (Vector2){0, 0}, 0.0f,
                 Fade(WHITE, pauseScreenOpeningProgress));

  if (pausedScreenOpen) {
    DrawTextEx(font, "GAME PAUSED",
               (Vector2){pausedScreenRec.x + 45, pausedScreenRec.y + 25}, 55, 5,
               WHITE);

    DrawTextEx(font, "Volume",
               (Vector2){pausedScreenRec.x + 160, pausedScreenRec.y + 95}, 45,
               3, quitButtonColor);

    DrawVolumeSlider(mousePos, &game.masterVolume);

    DrawRectangleLinesEx(playGameplayMusicRec, 5.0, quitButtonColor);

    DrawTextEx(font, "Play Arena Music",
               (Vector2){playGameplayMusicRec.x + 100, playGameplayMusicRec.y},
               30, 3, quitButtonColor);

    if (playGameplayMusic) {
      DrawRectangle(playGameplayMusicRec.x + 5, playGameplayMusicRec.y + 5,
                    playGameplayMusicRec.width - 10,
                    playGameplayMusicRec.height - 10, WHITE);
    }

    DrawRectangleRounded(quitToMainMenuButtonRec, 0.25, 1,
                         isHovering(quitToMainMenuButtonRec) ? secondaryColor
                                                             : quitButtonColor);
    DrawTextEx(
        font, TextFormat("Main \nMenu"),
        (Vector2){quitToMainMenuButtonRec.x + 30, quitToMainMenuButtonRec.y},
        30, 2, WHITE);
  }
}

// TIMED EVENT FUNCTIONS
void resetTimedEvent(TimedEvent *event, float delay, Sound soundEffect) {
  event->triggered = false;
  event->delay = delay;
  event->timer = 0.0f;
  event->particleTriggered = false;

  event->soundEffect = soundEffect;
}

bool updateTimedEvent(TimedEvent *event) {
  if (!event->triggered) {
    event->timer += GetFrameTime();

    if (event->timer >= event->delay) {
      event->triggered = true;
      PlaySound(event->soundEffect);
    }
  }

  return event->triggered;
}

// CAMERA FUNCTIONS
void cameraShake() {
  if (screenShake > 0) {
    screenShake--;
    camera.offset = (Vector2){
        camera.offset.x + GetRandomValue(-screenShakeValue, screenShakeValue),
        camera.offset.y + GetRandomValue(-screenShakeValue, screenShakeValue)};
  } else {
    // camera.offset = (Vector2){GAME_WIDTH/2, GAME_HEIGHT/2};
    camera.offset.x = Lerp(camera.offset.x, GAME_WIDTH / 2, 5 * GetFrameTime());
    camera.offset.y =
        Lerp(camera.offset.y, GAME_HEIGHT / 2, 5 * GetFrameTime());
  }
}

void updateCamera() {

  Vector2 mouseDir = {worldMouse.x - player.pos.x, worldMouse.y - player.pos.y};
  mouseDir = Vector2Normalize(mouseDir);
  mouseDir = Vector2Scale(mouseDir, 80);

  Vector2 desiredTarget = {player.pos.x + mouseDir.x,
                           player.pos.y + mouseDir.y};

  camera.target.x =
      (Lerp(camera.target.x, desiredTarget.x, 10 * GetFrameTime()));
  camera.target.y =
      (Lerp(camera.target.y, desiredTarget.y, 10 * GetFrameTime()));

  cameraShake();
}

// MENU STATE FUNCTIONS
void menuUpdate() {
  switch (game.menuState) {
  case MAIN:
    if (IsKeyPressed(KEY_ENTER)) {
      PlaySound(selectSound);
      transitionInit(&trans, 1, 0.25);
    }
    if (IsKeyPressed(KEY_TAB)) {
      PlaySound(selectSound);
      game.menuState = GUIDE;
    }

    startTextPos.y += GetFrameTime() * textBobbingSpeed;
    guideTextPos.y += GetFrameTime() * textBobbingSpeed;

    if (startTextPos.y >= 340 || startTextPos.y < 325) {
      textBobbingSpeed *= -1;
    }

    if (menuBoomerangRec.x >= 1080 || menuBoomerangRec.x < 125) {
      menuBoomerangSpeed *= -1;
    }

    break;
  case GUIDE:
    if (IsKeyPressed(KEY_TAB)) {
      SetSoundPitch(selectSound, GetRandomValue(3, 5) * 0.2);
      PlaySound(selectSound);
      game.menuState = MAIN;
    }
    break;
  }
}

void menuDraw() {
  switch (game.menuState) {
  case MAIN:
    DrawTexturePro(mainMenuTexture, (Rectangle){0, 0, 1280, 720},
                   (Rectangle){0, 0, 1280, 720}, (Vector2){0, 0}, 0.0f, WHITE);
    DrawTextEx(font, "[ENTER] - START GAME", startTextPos, 40, 3,
               quitButtonColor);
    DrawTextEx(font, "[TAB] - GUIDE SCREEN", guideTextPos, 40, 3,
               quitButtonColor);

    if (game.highScore > 0) {
      DrawTextEx(font, "HIGHSCORE:", (Vector2){475, 510}, 50, 3,
                 (Color){162, 38, 51, 255});

      Vector2 size =
          MeasureTextEx(font, TextFormat("%d", game.highScore), 50, 3);

      DrawRectangle(475 + 315, 510, size.x + 15, 50, quitButtonColor);
      DrawTextEx(font, TextFormat("%d", game.highScore),
                 (Vector2){475 + 325, 510}, 50, 3, WHITE);
    }
    playAnimation(&player.axe.anim, menuBoomerangRec, 1, 0.05f);
    break;

  case GUIDE:
    DrawTexture(guideScreenTexture, 0, 0, WHITE);
    break;
  }
}

// PLAYING STATE FUNCTIONS
void gamePlayingUpdate() {
  gameSetFullscreen();

  if (game.startGame) {
    updateScore();
    // GAMEPLAY MUSIC PLAYING
    if (playGameplayMusic) {
      updateGameplayMusic();
    }
  } else {
    UpdateMusicStream(idleMusic);
  }

  updateMusicVolume();

  if (hitStopTimer <= 0) {

    if (IsKeyPressed(KEY_ENTER)) {
      game.startGame = true;
    }
    if (game.startGame) {
      spawnEnemies();
    }

    int enemyUpdateReturn =
        enemyUpdate(enemy, player.rec, player.axe, player.pos,
                    game.colliderRecs, game.colliderCount, &ps);
    // Returns -1 - Enemy hit
    // Returns 1 - Enemy dead
    // Returns 2 - Shield enemy hit during throw

    if (enemyUpdateReturn == -1) { // Enemy hit
      screenShake = screenShakeFrameBase;
      hitStopTimer = hitStopTime * 2;

      SetSoundPitch(enemyHitSound, GetRandomValue(3, 4) * 0.45);
      PlaySound(enemyHitSound);
    }
    // Enemy killed and spawn particles
    else if (enemyUpdateReturn == 1) {

      game.enemiesKilled++;
      spawnParticlesExpandingRing(
          &ps, player.axe.pos, 0.25, enemy->healthBarColor, 5,
          GetRandomValue(10, 20),
          GetRandomValue(30,
                         50)); //(ps, spawnPos, lifeSpan, color, startSize,
                               // expandingRate, ring thickness)
    }
    // Shield Enemy is hit during throw
    else if (enemyUpdateReturn == 2 && !isBoomerangDeflected) {
      deflectAxe(&player);
      isBoomerangDeflected = true;
      spawnParticlesExpandingRing(&ps, player.axe.pos, 0.2, WHITE, 5, 10, 20);
      screenShake = 20;
      SetSoundPitch(deflectSound, GetRandomValue(3, 5) * 0.20);
      PlaySound(deflectSound);
    }
    // Reset deflected boolean
    if (player.axe.state != THROWN) {
      isBoomerangDeflected = false;
    }

    // If player hurt and screenshake hasnt started, start it once
    if (player.state == HURT && !playerScreenShakeStarted) {
      playerScreenShakeStarted = true;
      screenShake = screenShakeFrameBase + 10;
      hitStopTimer = hitStopTime;
    }

    // Reset screenshake check variable if player isnt hurt anymore
    if (player.state != HURT) {
      playerScreenShakeStarted = false;
    }

    // Start screenshakle if player throws boomerang
    if (player.justThrown) {
      screenShake = boomerangThrownScreenShake;
    }
    int checkEnemyAttackReturn = checkEnemyAttack();

    // Player update takes in enemy attack rectangle and also bool to see if
    // any enemies are attacking the checkEnemyAttack function returns an
    // index with the enemy attacking if one is, returns -1 otherwise We use
    // this index to pass in the proper attack rectangle and create a
    // booleon to pass in too
    playerUpdate(&player, game.colliderRecs, game.colliderCount,
                 enemy[checkEnemyAttackReturn].attackRec,
                 (checkEnemyAttackReturn != -1),
                 enemy[checkEnemyAttackReturn].pos);
    updateCamera();

    manageParticles();

  } else {
    hitStopTimer -= GetFrameTime();
  }
}

void gamePlayingDraw() {

  BeginMode2D(camera);

  // FLoor
  drawFloor(game.floorArray);
  DrawTexturePro(floorBloodTileTexture, (Rectangle){0, 0, 16, 16},
                 (Rectangle){500, 100, 100, 100}, (Vector2){0, 0}, 0.0f, WHITE);
  DrawTexturePro(floorBloodTileTexture, (Rectangle){0, 0, 16, 16},
                 (Rectangle){700, 100, 100, 100}, (Vector2){0, 0}, 0.0f, WHITE);

  // Walls
  drawLevel(game.levelArray, 0);
  // Props
  drawLevel(game.propsArray, 1);

  playerDraw(&player, game.state == DYING_TRANSITION, game.state == DEAD);

  enemyDraw(enemy,
            game.state == UPGRADE_SCREEN || game.state == DYING_TRANSITION);

  if (game.state == PLAYING) {
    drawParticles(&ps);
  }

  if (game.state == DYING_TRANSITION || game.state == DEAD) {

    if (ringDistance > 100)
      ringDistance -= 15;

    DrawRing(player.pos, ringDistance, 10000, 0, 360, 1, BLACK);
  } else
    ringDistance = 1280;

  EndMode2D();

  // Draw player health
  for (int i = 0; i < 3; i++) {
    int threshold = (i + 1) * 2;
    Texture2D tex;

    if (player.lives >= threshold) {
      tex = heartTexture;
    } else if (player.lives == threshold - 1) {
      tex = heartHalfTexture;
    } else {
      tex = heartEmptyTexture;
    }

    DrawTexturePro(tex, (Rectangle){0, 0, 16, 16}, healthRecs[i],
                   (Vector2){0, 0}, 0.0, WHITE);
  }

  // Score display
  if (game.state != DEAD) {
    DrawTexture(scoreDisplayTexture, 1125, 5, WHITE);

    Vector2 size = MeasureTextEx(font, TextFormat("%d", game.score), 50, 0);
    float xPos = 1200 - (31.25 / 2 * (size.x / 31.25));
    DrawTextEx(font, TextFormat("%d", game.score), (Vector2){xPos, 17.5}, 50, 0,
               quitButtonColor);
  }

  // Start game display
  if (!game.startGame) {
    DrawTextEx(font, "[ENTER] - START",
               (Vector2){1280 / 2 - 225, 720 / 2 - 275}, 50, 4, WHITE);
  }
}

// DEAD STATE FUNCTIONS
void gameUpdateDeadScreen() {

  if (!hasFallen) {
    gameOverRecVelY += gameOverRecGravity;
  }

  if (gameOverRec.y >= 160) {
    gameOverRecVelY *= 0.75f;
    gameOverRecVelY *= -1;
    bounceCount++;

    if (!hasFallen) {
      SetSoundPitch(gameOverScreenFallSound, GetRandomValue(4, 5) * 0.20f);
      PlaySound(gameOverScreenFallSound);
    }
  }

  if (bounceCount >= 4) {
    gameOverRec.y = 160;
    hasFallen = true;
    gameOverRecVelY = 0.0f;
    bounceCount = 0;
  }

  // Update rec
  gameOverRec.y += gameOverRecVelY;

  if (hasFallen) {
    if (updateTimedEvent(&displayTimeSurvied)) {
      if (updateTimedEvent(&displayEnemiesKilled)) {
        if (updateTimedEvent(&displayScore)) {
          gameOverAnimationsDone = true;

          if (game.score > game.highScore) {
            game.highScore = game.score;
            SaveHighScore(game.highScore);
            isNewHighScore = true;
          }
        }
      }
    }
  }

  if (gameOverAnimationsDone) {

    if (isHovering(playAgainButtonRec)) {
      playAgainButtonRec = (Rectangle){
          .x = playAgainButtonRecBase.x - 2.5,
          .y = playAgainButtonRecBase.y - 2.5,
          .width = playAgainButtonRecBase.width + 5,
          .height = playAgainButtonRecBase.height + 5,
      };
      playAgainButtonColor = hightlightColor;

      if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        SetSoundPitch(selectSound, GetRandomValue(3, 5) * 0.2);
        PlaySound(selectSound);
        transitionInit(&trans, TO_PLAYING, 0.25);
      }

    } else {
      playAgainButtonRec = playAgainButtonRecBase;
      playAgainButtonColor = buttonColor;
    }

    if (isHovering(quitButtonRec)) {
      quitButtonColor = hightlightColor;
      quitButtonRec = (Rectangle){
          .x = quitButtonRecBase.x - 2.5,
          .y = quitButtonRecBase.y - 2.5,
          .width = quitButtonRecBase.width + 5,
          .height = quitButtonRecBase.height + 5,
      };

      if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        SetSoundPitch(selectSound, GetRandomValue(3, 5) * 0.2);
        PlaySound(selectSound);
        transitionInit(&trans, TO_MENU, 0.25);
      }
    } else {
      quitButtonColor = buttonColor;
      quitButtonRec = quitButtonRecBase;
    }
  }
}

void gameDeadScreenDraw() {

  DrawRectangle(0, 0, 1280, 720, Fade(BLACK, 0.4));

  DrawTexturePro(gameOverTexture, (Rectangle){0, 0, 80, 64}, gameOverRec,
                 (Vector2){0, 0}, 0.0f, WHITE);
  DrawTextEx(font, "GAME OVER",
             (Vector2){gameOverRec.x + 60, gameOverRec.y + 20}, 64, 5,
             (Color){162, 38, 51, 255});

  DrawTextEx(font, "Time Survived:",
             (Vector2){gameOverRec.x + 30, gameOverRec.y + 100}, 30, 2,
             (Color){24, 20, 37, 255});

  DrawTextEx(font, "Enemies Killed:",
             (Vector2){gameOverRec.x + 30, gameOverRec.y + 150}, 30, 2,
             (Color){24, 20, 37, 255});

  DrawTextEx(font, "FINAL SCORE",
             (Vector2){gameOverRec.x + 125, gameOverRec.y + 210}, 34, 3,
             (Color){24, 20, 37, 255});

  if (displayTimeSurvied.triggered) {
    int minutes = (int)game.timeSurvived / 60;
    int secs = (int)game.timeSurvived % 60;

    DrawTextEx(font, TextFormat("%d:%02d", minutes, secs),
               (Vector2){gameOverRec.x + 300, gameOverRec.y + 100}, 30, 2,
               WHITE);
    if (!displayTimeSurvied.particleTriggered) {
      spawnParticlesExpandingRing(
          &ps, (Vector2){gameOverRec.x + 300 + 25, gameOverRec.y + 100 + 10},
          0.5, WHITE, 5, 5, 50);
      screenShake = screenShakeFrameBase + 10;
      displayTimeSurvied.particleTriggered = true;
    }
  }

  if (displayEnemiesKilled.triggered) {
    DrawTextEx(font, TextFormat("%d", game.enemiesKilled),
               (Vector2){gameOverRec.x + 300, gameOverRec.y + 150}, 30, 2,
               WHITE);
    if (!displayEnemiesKilled.particleTriggered) {
      spawnParticlesExpandingRing(
          &ps, (Vector2){gameOverRec.x + 300 + 10, gameOverRec.y + 150 + 5},
          0.5, WHITE, 5, 5, 50);
      screenShake = screenShakeFrameBase + 10;
      displayEnemiesKilled.particleTriggered = true;
    }
  }

  if (displayScore.triggered) {
    Vector2 size = MeasureTextEx(font, TextFormat("%d", game.score), 50, 3);
    float xPos = (gameOverRec.x + 255) - (31.25 / 2 * (size.x / 31.25));

    DrawTextEx(font, TextFormat("%d", game.score),
               (Vector2){xPos, gameOverRec.y + 250}, 50, 3, WHITE);

    if (!displayScore.particleTriggered) {
      spawnParticlesExpandingRing(
          &ps, (Vector2){gameOverRec.x + 225 + 10, gameOverRec.y + 250 + 10},
          0.75, WHITE, 5, 15, 50);
      screenShake = screenShakeFrameBase + 10;
      displayScore.particleTriggered = true;
    }

    // new high score display
    if (isNewHighScore) {
      newHighScoreDisplayTimer += GetFrameTime();
      if (newHighScoreDisplayTimer >= 1.5f) {
        newHighScoreFrameCount++;
        Color textColor;

        if (newHighScoreFrameCount >= 16) {
          textColor = quitButtonColor;
          if (newHighScoreFrameCount >= 20) {
            newHighScoreFrameCount = 0;
          }
        } else {
          textColor = GOLD;
        }

        DrawTextEx(font, "NEW HIGHSCORE",
                   (Vector2){gameOverRec.x + gameOverRec.width / 6,
                             gameOverRec.y + gameOverRec.height},
                   40, 3, textColor);
      }
      if (newHighScoreDisplayTimer >= 1.3f &&
          newHighScoreDisplayTimer <= 1.5f) {
        PlaySound(newHighScoreDisplaySound);
      }
    } else {
      newHighScoreDisplayTimer = 0.0f;
      newHighScoreFrameCount = 0;
    }
  }

  if (displayScore.triggered) {
    DrawRectangleRounded(quitButtonRec, 0.5, 1, quitButtonColor);
    DrawRectangleRoundedLinesEx(quitButtonRec, 0.5, 1, 2.5, secondaryColor);

    DrawRectangleRounded(playAgainButtonRec, 0.5, 1, playAgainButtonColor);
    DrawRectangleRoundedLinesEx(playAgainButtonRec, 0.5, 1, 2.5,
                                secondaryColor);

    DrawTextEx(font, "QUIT",
               (Vector2){quitButtonRec.x + 25, quitButtonRec.y + 10}, 40, 2,
               WHITE);
    DrawTextEx(font, "PLAY\nAGAIN",
               (Vector2){playAgainButtonRec.x + 25, playAgainButtonRec.y}, 30,
               2, WHITE);
  }
}

// GAME FUNCTIONS
void gameUpdate() {
  transitionUpdate(&trans, &game.state);

  switch (game.state) {
  case MAIN_MENU:
    menuUpdate();
    updateParticles(&ps);
    updateMenuMusic();
    updateMusicVolume();
    break;
  case PLAYING:

    if (!game.startGame) {
    }

    if (IsKeyPressed(KEY_ESCAPE)) {
      game.state = PAUSED;
    }

    gamePlayingUpdate();
    if (startUpgrades) {

      game.state = UPGRADE_SCREEN;
      createUpgrades(&upgradeScreen,
                     player.upgradeLevels[IMMUNE_WHILE_PULLING_IN] == 1,
                     player.lives == 6);
      resetUpgradeUI(&upgradeScreen);
    }
    if (game.score >= (game.scoreThresholdNum * 125)) {
      PlaySound(upgradeUnlockedSound);
      startUpgrades = true;
      game.scoreThresholdNum++;
    } else {
      startUpgrades = false;
    }

    // Player died
    if (player.lives <= 0) {
      game.state = DYING_TRANSITION;
    }

    // DEBUG
    if (IsKeyPressed(KEY_P)) {
      player.lives--;
    }

    break;
  case DYING_TRANSITION:
    playDeadAnimationTimer += GetFrameTime();

    if (!isGameOverSoundPlayed) {
      PlaySound(gameOverSound);
      isGameOverSoundPlayed = true;
    }
    if (playDeadAnimationTimer >= 4.5f) {
      game.state = DEAD;
      isGameOverSoundPlayed = false;
    }
    break;
  case DEAD:
    gameUpdateDeadScreen();
    updateParticles(&ps);
    cameraShake();
    break;
  case UPGRADE_SCREEN:
    updateUpgradeScreen(&upgradeScreen, mousePos);

    if (upgradeScreen.selectedUpgrade != -1) {
      if (applyPlayerUpgrade(&player, upgradeScreen.selectedUpgrade)) {
        game.state = PLAYING;
        upgradeScreen.selectedUpgrade = -1;
      }
    }
    break;
  case PAUSED:
    gameUpdatePausedScreen();
    if (IsKeyPressed(KEY_ESCAPE)) {
      pausedScreenOpen = false;
      pauseScreenOpeningProgress = 0.0f;
      game.state = PLAYING;
    }
    break;
  }
}

void gameSetFullscreen() {
  if (IsKeyPressed(KEY_F11)) {
    ToggleBorderlessWindowed();
  }
}

void gameResolutionDraw() {
  BeginDrawing();
  ClearBackground(WHITE);

  float scale = fmin((float)GetScreenWidth() / GAME_WIDTH,
                     (float)GetScreenHeight() / GAME_HEIGHT);

  float scaledWidth = GAME_WIDTH * scale;
  float scaledHeight = GAME_HEIGHT * scale;

  float offsetX = (GetScreenWidth() - scaledWidth) * 0.5f;
  float offsetY = (GetScreenHeight() - scaledHeight) * 0.5f;

  Rectangle src = {
      0, 0, (float)target.texture.width,
      -(float)target.texture.height // flip vertically
  };

  Rectangle dst = {offsetX, offsetY, scaledWidth, scaledHeight};

  DrawTexturePro(target.texture, src, dst, (Vector2){0, 0}, 0.0f, WHITE);

  mousePos = GetMousePosition();

  mousePos.x = (mousePos.x - offsetX) / scale;
  mousePos.y = (mousePos.y - offsetY) / scale;
  worldMouse = GetScreenToWorld2D(mousePos, camera);

  EndDrawing();
}

void gameDraw() {
  BeginTextureMode(target);

  ClearBackground(BLACK);

  switch (game.state) {
  case MAIN_MENU:
    menuDraw();
    drawParticles(&ps);
    break;
  case PLAYING:
    gamePlayingDraw();
    break;
  case DYING_TRANSITION:

    gamePlayingDraw();

    break;
  case DEAD:
    gamePlayingDraw();
    gameDeadScreenDraw();
    drawParticles(&ps);
    break;
  case UPGRADE_SCREEN:
    gamePlayingDraw();
    gameUpgradeScreenDraw();
    break;
  case TESTING:
    drawParticles(&ps);
    break;
  case PAUSED:
    gamePlayingDraw();
    gameDrawPausedScreen();
  }

  // Draw cursor

  DrawTexturePro(cursorTexture, (Rectangle){0, 0, 16, 16},
                 (Rectangle){mousePos.x - 15, mousePos.y - 15, 30, 30},
                 (Vector2){0, 0}, 0.0f, WHITE);

  transitionDraw(&trans);

  EndTextureMode();

  gameResolutionDraw();
}
