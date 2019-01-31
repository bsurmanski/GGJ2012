/*
 * main.c
 * GameJam
 * January 27, 2012
 * Brandon Surmanski
 */

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <math.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include "planet.h"
#include "actor.h"
#include "sprite.h"
#include "particle.h"
#include "actor.h"

int SCREENX = 320;
int SCREENY = 240;
double xoffset = 0;
double yoffset = 0;

int running;
int seed;
SDL_Surface *scaled;
SDL_Surface *titlesurf;
SDL_Surface *s;
Actor *player;

extern float scale[];// = {0.025f};
extern float speed[];
extern double radius;
extern int RAD_MAX;
extern int RAD_MIN;
int flicker = 0;
int title = 1;

void upscaleCopy(SDL_Surface *dest, SDL_Surface *src, int scale)
{
    //SDL_BlitScaled(src, 0, dest, 0);
    if (SDL_MUSTLOCK(dest)) SDL_LockSurface(dest);
    int i, j, k;
    for(j = 0; j < src->h; j++){
        for(i = 0; i < src->w; i++){
            for(k = 0; k < scale; k++){
                memcpy(dest->pixels + flicker * k * dest->format->BytesPerPixel + scale * ((i * dest->format->BytesPerPixel) + j * dest->pitch),
                        src->pixels + (i * src->format->BytesPerPixel + j * src->pitch),
                      src->format->BytesPerPixel);
            }
        }
        /*for(k = 1; k < scale; k++){
        memcpy(dest->pixels + scale * (j * dest->pitch),
                dest->pixels + scale * ((j+k) * dest->pitch),
                dest->pitch
                );
        }*/
    }
    if (SDL_MUSTLOCK(dest)) SDL_UnlockSurface(dest);
    //return;
}

int inSurface(SDL_Surface *s, int x, int y)
{
    return (x>0 && x<s->w && y>0 && y<s->h);
}

void setPixel(SDL_Surface *s, int x, int y, int color)
{
    if(inSurface(s,x,y)){
        memcpy(
                s->pixels + x * s->format->BytesPerPixel + y * s->pitch,
                &color,
                s->format->BytesPerPixel
                );
    }
}

int getPixel(SDL_Surface *s, int x, int y)
{
    int ret = 0;
    if(inSurface(s,x,y)){
        memcpy(&ret,
                s->pixels + x * s->format->BytesPerPixel + y * s->pitch,
                s->format->BytesPerPixel);
    }
    return ret;
}

void init(void)
{
    int i, j;
    SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO);
    scaled = SDL_SetVideoMode(SCREENX*2, SCREENY*2, 32, SDL_SWSURFACE);

    s = SDL_CreateRGBSurface(SDL_SWSURFACE,
            SCREENX,
            SCREENY,
            scaled->format->BitsPerPixel,
            scaled->format->Rmask,
            scaled->format->Gmask,
            scaled->format->Bmask,
            scaled->format->Amask
            );

    titlesurf = IMG_Load("res/title.png");


    srand(time(0));
    seed = rand();

    scale[0] = 0.025f;
    speed[0] = 0.005f;
    for(i = 1; i < ACTORS; i++){
        scale[i] = scale[i-1]/8.0f;
        speed[i] = speed[i-1]/4.0f;
    }

    radius = 1.0f / scale[0];

    sprite_init();
    player = actor_new(egg-1);
    particle_init();
    float pos[2] = {0.0f, 1.0f};
    float vel[2] = {0.0f, 0.0f};
}

extern double angle;
void handleEvents(void)
{
    SDL_PumpEvents();
    const uint8_t *keystate = SDL_GetKeyboardState(NULL);
    SDL_Event event;
     while( SDL_PollEvent( &event ) ){
            //If the user has Xed out the window
        if( event.type == SDL_QUIT ){
            running = 0;
        }
    }
    //if(keystate[SDLK_ESCAPE])
    //    running = 0;

    if(title){
        if(keystate[SDLK_SPACE] || keystate[SDLK_RETURN]) {
            title = 0;
        }
    } else {
        if(keystate[SDL_SCANCODE_Q])
            actor_kill(player);

        if(keystate[SDLK_LEFT] && !player->rebirth){
            angle+=speed[player->state];
            player->left = 1;
            actor_animate(player);
        }
        if(keystate[SDLK_RIGHT] && !player->rebirth){
            angle-=speed[player->state];
            player->left = 0;
            actor_animate(player);
        }

        if(keystate[SDLK_SPACE])
            actor_crack(player);
    }
}

void update(void)
{
    static int time = 0;
    if(SDL_GetTicks() - time > 16){
        flicker = !flicker;
        time = SDL_GetTicks();

        float pos[2] = {0.0f, 1.0f};
        float vel[2] = {0.0f, 0.0f};

        float ang = angle + (1.0f -  2.0f*(float)rand() / (float) RAND_MAX) * log(scale[player->state]) * - 5;
        float rad = 175.0f + 250 * exp(scale[player->state] / scale[elephant]);
        pos[0] = cos(3.0f*PI/2.0f-ang) * rad;
        pos[1] = sin(3.0f*PI/2.0f-ang) * rad;
        particle_add(pos, vel);
    }
    if(!title){
        actor_update(player);
        particle_update();
    }
}

void draw(void)
{
    SDL_FillRect(scaled, 0, SDL_MapRGB(scaled->format, 0, 0, 0));
    if(title){
        upscaleCopy(scaled, titlesurf, 2);
    } else {
        SDL_FillRect(s, 0, SDL_MapRGB(s->format, 0, 0, 0));
        if (SDL_MUSTLOCK(s)) SDL_LockSurface(s);
        planet_draw(s);
        particle_draw(s);
        if (SDL_MUSTLOCK(s)) SDL_UnlockSurface(s);
        actor_draw(player, s);
        upscaleCopy(scaled, s, 2);
    }
}

void tick() {
    handleEvents();
    update();
    draw();
    //SDL_FillRect(scaled, 0, SDL_MapRGB(scaled->format, 255, 0, 0));
    SDL_Flip(scaled);
}


void run(void)
{
    running = 1;
    int time = SDL_GetTicks();
    int delay;

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(tick, 0, 1);
#else
    while(running){
        tick();
        //delay
        delay = 16 - (SDL_GetTicks() - time);
        if(delay > 0)
            SDL_Delay(delay);
        time = SDL_GetTicks();
    }
#endif
}


int main(int argc, char **argv)
{
    init();
    run();
    return 0;
}
