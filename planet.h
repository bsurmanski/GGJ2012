/**
 * planet.h
 * GameJam
 * January 27, 2012
 * Brandon Surmanski
 */

#ifndef _PLANET_H
#define _PLANET_H

#include <SDL/SDL.h>

#define PI 3.1415926535897932384626433832795028841971


void planet_draw(SDL_Surface *s);
double planet_getSlope(double rad);
double planet_getRadius(double rad);

#endif
