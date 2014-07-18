/**
 * sprite.h
 * GameJam
 * January 28, 2012
 * Brandon Surmanski
 */

#ifndef _SPRITE_H
#define _SPRITE_H

#include "actor.h"
#define ACTORS 10

void sprite_init(void);
SDL_Surface **sprite_get(enum State s);

#endif
