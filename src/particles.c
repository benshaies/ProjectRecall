#include "../headers/particles.h"
#include "stdio.h"

void spawnParticles(ParticleSystem *ps, Vector2 pos, float lifeMax, Color color, Vector2 velocity, float size){
    
    for(int i = 0; i < MAX_PARTICLES; i++){
        if(!ps->pool[i].active){
            ps->pool[i] = (Particle){
                .pos = pos,
                .life = lifeMax,
                .lifeMax = lifeMax,
                .color = color,
                .velocity = velocity,
                .size = size,
                .active = true,
            };
            printf("%d, %f, %f\n", i, ps->pool[i].pos.x, ps->pool[i].size);
            return;
        }
    }

}


void updateParticles(ParticleSystem *ps){
    for(int i = 0; i < MAX_PARTICLES; i++){

        if(!ps->pool[i].active) continue;
        ps->pool[i].pos.x += ps->pool[i].velocity.x;
        ps->pool[i].pos.y += ps->pool[i].velocity.y;

        ps->pool[i].life -= GetFrameTime();



        if(ps->pool[i].life <= 0.0f){
            ps->pool[i].active = false;
        }
    }
}

void drawParticles(ParticleSystem *ps){
    for(int i = 0; i < MAX_PARTICLES; i++){
        if(!ps->pool[i].active) continue;
        DrawCircleV(ps->pool[i].pos, ps->pool[i].size, ps->pool[i].color);
    }
}