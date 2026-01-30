#ifndef PLAYER_H
#define PLAYER_H
#include "raylib.h"
#include "animation.h"

    typedef enum{
        IDLE,
        PULLING_IN,
    }State;

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
    }Player;

    void playerInit(Player *player);

    void playerUpdate(Player *player);

    void playerMovement(Player *player);

    void axeUpdate(Player *player);

    void playerDraw(Player *player);


#endif
