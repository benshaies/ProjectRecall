#ifndef PARTICLES_H
#define PARTICLES_H
#include <raylib.h>
#define MAX_PARTICLES 1024
    
    typedef struct{
       Vector2 pos;
       bool active;
       float life;
       float lifeMax;
       Color color;
       Vector2 velocity;
       float size;
    }Particle;

    typedef struct{
        Particle pool[MAX_PARTICLES];
    }ParticleSystem;

    void spawnParticles(ParticleSystem *ps, Vector2 pos, float lifeMax, Color color, Vector2 velocity, float size);

    void updateParticles(ParticleSystem *ps);

    void drawParticles(ParticleSystem *ps);

#endif
