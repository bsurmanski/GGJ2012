/**
 * particle.c
 * GameJam
 * January 27, 2012
 * Brandon Surmanski
 */

#include <SDL/SDL.h>
#include <float.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "list.h"
#include "actor.h"

#include "particle.h"

extern int offsetx;
extern int offsety;
extern double xoffset;
extern double yoffset;
extern double radius;
extern SDL_Surface *s;
extern void setPixel(SDL_Surface *s, int x, int y, int color);
extern int getPixel(SDL_Surface *s, int x, int y);
extern void actor_damage(Actor *a);
List *pList;
extern Actor *player;

void particle_init(void)
{
    pList = list_new(sizeof(Particle));
}

//void particle_clear(List *p)
//{
//    p->first = 0;
//}


int particle_update()
{
    if (SDL_MUSTLOCK(s)) SDL_LockSurface(s);
    Particle *p;
    int remove = 0;
    while((p = list_itter(pList))){
        if(fabs(p->lastPos[0] - p->position[0]) > 20
               || fabs(p->lastPos[1] - p->position[1]) > 20){
            float theta = 0;
            if(fabs(p->position[0]) > FLT_EPSILON || fabs(p->position[1]) > FLT_EPSILON){
                theta = atan2(p->position[1], p->position[0]);
                float radsq = radius; //(planet_getRadius(theta) / 150.0f);
                radsq *= radsq;
                float px = p->position[0] * radius / 150.0f;
                float py = p->position[1] * radius / 150.0f;
                float possq = px * px + py * py;
            if(possq < radsq){
                remove = 1;
            }
                p->lastPos[0] = p->position[0];
                p->lastPos[1] = p->position[1];
            }
            float accel[2] = {-cos(theta) / 1000.0f, -sin(theta) / 1000.0f};
            memcpy(p->acceleration, accel, sizeof(float) * 2);
        }
        p->velocity[0] += p->acceleration[0];
        p->velocity[1] += p->acceleration[1];
        p->position[0] += p->velocity[0];
        p->position[1] += p->velocity[1];
        p->life--;
        if(remove || p->life < 0){
            remove = 0;
            list_removeLastItter(pList);
            actor_kill(player);
        }

        int x = (int)(p->position[0] * (radius / 150.0f) + xoffset);
        int y = (int)(p->position[1] * (radius / 150.0f) + yoffset);
        int pxl = getPixel(s, x, y);
        if(pxl != 0xFFFFFFFF && pxl != 0x00000000)
        {
            // this doesn't work well with emscripten
            //actor_kill(player);
        }
    }

    if (SDL_MUSTLOCK(s)) SDL_UnlockSurface(s);

    return 0;
    //return (p->life >= 0);
}

void particle_add(float *pos, float *vel)
{
    Particle *p = malloc(sizeof(Particle));
    memcpy(p->position, pos, sizeof(float) * 2);
    memcpy(p->lastPos, pos, sizeof(float) * 2);
    memcpy(p->velocity, vel, sizeof(float) * 2);
    memset(p->acceleration, 0, sizeof(float) * 2);
    float theta = 0;
    if(fabs(p->position[0]) > FLT_EPSILON || fabs(p->position[1]) > FLT_EPSILON){
        theta = atan2(p->position[1], p->position[0]);
    }
    float accel[2] = {-cos(theta) / 500.0f, -sin(theta) / 500.0f};
    memcpy(p->acceleration, accel, sizeof(float) * 2);

    p->life = 1000000;
    list_add(pList, p);
    free(p);
}

void particle_draw(SDL_Surface *s)
{
    Particle *p;
    int i = 0;
    while(p = list_itter(pList)){
        int x = (int)(p->position[0] * (radius / 150.0f) + xoffset);
        int y = (int)(p->position[1] * (radius / 150.0f) + yoffset);
        setPixel(s, x, y, 0xFFFFFFFF);
    }
}
