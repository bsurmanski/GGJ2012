/**
 * actor.c
 * GameJam
 * January 27, 2012
 * Brandon Surmanski
 */

#include <math.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "planet.h"
#include "actor.h"
#include "matrix.h"
#include "sprite.h"
#include "list.h"

SDL_Surface *snake0;
SDL_Surface *snake1;
SDL_Surface *activeSprite;
extern SDL_Surface *rotbuf;
extern double easeRadius;

float scale[ACTORS];
float speed[ACTORS];
//elephant man, snake, bee, ant, egg
int animSprites[ACTORS] = {2, 4, 2, 2, 2, 3, 0, 0};

Actor *actor_new(enum State s)
{
    Actor *p = malloc(sizeof(Actor));
    p->x = 0.0f;
    p->y = 1.0f;
    p->state = s;
    p->activeSprite = 0;
    p->left = 0;
    p->rebirth = 1;
    easeRadius = 1.0f / scale[s];
    p->life = 1000;
    p->damaged = 0;
    return p;
}

extern int SCREENX;
extern int SCREENY;
extern double xoffset;
extern double yoffset;
extern double radius;
extern double TOP;
double angle = 0;
void actor_draw(Actor *p, SDL_Surface *s)
{
    int halfW = rotbuf->w / 2;
    int halfH = rotbuf->h / 2;
    float cosa = cos(PI/2.0f+angle) * planet_getRadius(TOP-angle);
    float sina = sin(PI/2.0f+angle) * planet_getRadius(TOP-angle);
    SDL_Rect srcRect = {0, 0, rotbuf->w, rotbuf->h};
    SDL_Rect destRect = {xoffset + cosa - halfW,
        yoffset  - sina - halfH,
        xoffset  +cosa + halfW,
        yoffset - sina + halfH};
    SDL_BlitSurface(rotbuf, &srcRect, s, &destRect);
}

void actor_animate(Actor *p)
{
    p->animate = 1;
}

void actor_update(Actor *p)
{
    static int time = 0;
    int looplim = animSprites[p->state];
    if(p->rebirth && p->crack) {
    looplim = animSprites[egg];
    p->animate = 1;
    }
    if(SDL_GetTicks() - time > 400 && p->animate){
        time = SDL_GetTicks();
        p->activeSprite++;
        if(p->activeSprite>looplim){
            p->activeSprite = 1;
            if(p->rebirth){p->rebirth = 0; p->crack = 0;}
        }
    }
    if(!p->animate)
        p->activeSprite = 0;
    p->animate = 0;

    SDL_FillRect(rotbuf, 0, 0);
    SDL_Surface *src;
    if (SDL_MUSTLOCK(rotbuf)) SDL_LockSurface(rotbuf);
    if(p->rebirth) src = sprite_get(egg)[p->activeSprite];
    else src = sprite_get(p->state)[p->activeSprite];

    if (SDL_MUSTLOCK(src)) SDL_LockSurface(src);
    matrix_rotateImage(src, rotbuf, planet_getSlope(TOP - angle) + angle, p->left);
    if (SDL_MUSTLOCK(src)) SDL_UnlockSurface(src);
    if (SDL_MUSTLOCK(rotbuf)) SDL_UnlockSurface(rotbuf);

    if(p->life <= 0)
        actor_kill(p);
    p->life--;
}

void actor_changeState(Actor *p, enum State s)
{
    p->state = s; //TODO: particles!
}

void actor_delete(Actor *p)
{
    free(p);
}

void actor_crack(Actor *p)
{
    if(p->rebirth)
        p->crack = 1;
}

extern List *pList;
void actor_kill(Actor *p)
{
    if(!p->rebirth && p->state != 0 && p->state !=blank){
        p->state--;
        p->rebirth = 1;
        p->activeSprite = 0;
        easeRadius = 1.0f / scale[p->state];
        p->life = 1000;
        list_clear(pList);
    }
    else if(p->state == 0 && !p->rebirth){
        p->state = blank;
        p->life = 1000;
        p->activeSprite = 0;
    }
}

void actor_damage(Actor *p)
{
    if(!p->rebirth){
        p->life--;
        p->damaged = 1;
    }
}




