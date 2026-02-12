#ifndef PLAYER_H
#define PLAYER_H
#include "raylib.h"
#include "animation.h"
#include "../headers/arena.h"

    typedef enum{
        NOTHING,
        PULLING_IN,
    }State;

    typedef enum{
        IDLE,
        RUNNING,
    }AnimState;

    typedef enum{
        HOLDING,
        THROWN,
        DONE_THROW,
        RECALL,
    }State2;

    typedef struct{
        Vector2 pos;
        Rectangle rec;
        float throwSpeed;
        float recallSpeed;
        State2 state;

        Vector2 attackPos;
        float attackCheckRadius;
        Vector2 dir;

        float damage;

        Animation anim;

    }Weapon;

    typedef struct{
        //Player specific
        Vector2 pos;
        Rectangle rec;
        float speed;
        Vector2 dir;
        State state;
        float health;
        float baseHealth;

        //Axe varaibles
        Weapon axe;

        //Animations
        Animation playerIdleAnim;
        Animation playerSideAnim;

        //Animation related variables
        int animationDir;
        AnimState animState;
    }Player;

    void playerInit(Player *player);

    void playerUpdate(Player *player, Rectangle rec[], int recNum);

    void playerMovement(Player *player);

    void playerCollisions(Player *player, Rectangle rec[], int recNum);

    void axeUpdate(Player *player, Rectangle rec[], int recNum);

    void playerDraw(Player *player);

#endif
