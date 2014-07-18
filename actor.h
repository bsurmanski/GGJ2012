/**
 * actor.h
 * GameJam
 * January 27, 2012
 * Brandon Surmanski
 */

#ifndef _actor_H
#define _actor_H

#include <SDL/SDL.h>

enum State{
    elephant,
    man,
    snake,
    bee,
    ant,
    egg,
    blank
};

enum Sprtes{
    numElephant=2,
    numMan=4,
    numSnake=2,
    numBee=2,
    numAnt=2,
    numEgg=3
};

typedef struct Actor{
    float x;
    float y;
    SDL_Surface *rotbuf;
    int activeSprite;
    enum State state;
    int rebirth;
    int left;
    int animate;
    int life;
    int damaged;
    int crack;
} Actor;

Actor *actor_new(enum State s);
void actor_init(void);
void actor_draw(Actor *p, SDL_Surface *s);
void actor_update(Actor *p);
void actor_kill(Actor *p);
void actor_damage(Actor *p);

#endif
