/**
 * sprite.c
 * GameJam
 * January 28, 2012
 * Brandon Surmanski
 */

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include "sprite.h"
#include "actor.h"

SDL_Surface *sprite[ACTORS][10];

SDL_Surface *rotbuf;

void sprite_load(char *file, SDL_Surface **buf, int num)
{
    int i;
    char filetemp[32];
    memset(filetemp, 0, 32);
    printf("F:%s\n",file);
    buf[0] = IMG_Load(file);
    int dotLoc = strchr(file, '.') - file;
    memcpy(filetemp, file, dotLoc);
    int numLoc = dotLoc;
    filetemp[numLoc+1] = '.';
    filetemp[numLoc+2] = 'p';
    filetemp[numLoc+3] = 'n';
    filetemp[numLoc+4] = 'g';
    for(i = 1; i <= num; i++)
    {
        filetemp[numLoc] = i + '0';
        printf("F:%s\n",filetemp);
        buf[i] = IMG_Load(filetemp);
    }
}

void sprite_init(void)
{
    sprite_load("res/elephant.png", sprite[elephant], 2);
    sprite_load("res/man.png", sprite[man], 4);
    sprite_load("res/snake.png", sprite[snake], 2);
    sprite_load("res/bee.png", sprite[bee], 2);
    sprite_load("res/ant.png", sprite[ant], 2);
    sprite_load("res/egg.png", sprite[egg], 3);
    sprite_load("res/blank.png", sprite[blank], 1);

    rotbuf = SDL_CreateRGBSurface(SDL_SWSURFACE,
            (int)(32 * 1.5), //1.5 APPROX = sqrt(2)
            (int)(32 * 1.5),
            sprite[0][0]->format->BitsPerPixel,
            sprite[0][0]->format->Rmask,
            sprite[0][0]->format->Gmask,
            sprite[0][0]->format->Bmask,
            sprite[0][0]->format->Amask
            );

}

SDL_Surface **sprite_get(enum State s)
{
    return sprite[s];
}


