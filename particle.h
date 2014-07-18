/**
 * particle.h
 * GameJam
 * January 27, 2012
 * Brandon Surmanski
 */

#ifndef _PARTICLE_H
#define _PARTICLE_H

#include <SDL/SDL.h>

typedef struct Particle {
    float position[2];
    float lastPos[2];
    float velocity[2];
    float acceleration[2];
    int life;
} Particle;

void particle_init(void);
int particle_update();
void particle_draw(SDL_Surface *s);
void particle_add(float *pos, float *vel);

#endif
