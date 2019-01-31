/**
 * planet.c
 * GameJam
 * January 27, 2012
 * Brandon Surmanski
 */

#include <SDL/SDL.h>
#include <math.h>

#include "planet.h"

#define NOISE_RATIO 0.1f
#define ROUGHNESS 10

extern void setPixel(SDL_Surface *s, int x, int y, int color);

extern int seed;
extern int SCREENX;
extern int SCREENY;
double radius = 50.0f;
double easeRadius = 150.f;
double TOP = 3.0f * PI / 2.0f;
double rot = 0.0f;

// 2^22. Things start to break down here due to floating precision
int RAD_MAX = 4194304;
int RAD_MIN = 2;

double randnum(int x)
{
    x+= seed;
    x = (x<<13) ^ x;
    double t = (1.0f - ((x * (x*x*15731 + 789221) + 1376312589) & 0x7fffffff)/1073741824.0f);
    return t;
}

double lerp(float t, float a, float b)
{
    return(a+t*(b-a));
}

double s_curve(double t)
{
    return (t*t*t*(10+t*(-15+(6*t))));
}

double noise(double x)
{
    int flx = floor(x);
    double dx0 = randnum(flx);
    double dx1 = randnum(flx + 1);
    return lerp(s_curve(x-flx), dx0, dx1);
}

double terb(double x, int n)
{
    float sum = 0;
    int i;
    for(i = 1<<n; i >0; i = i>>1)
        sum += fabs(noise(x*i)/i);
    return sum;
}


double noiseLoop(double x)
{
    double n0 = terb(x,ROUGHNESS);
    double n1 = terb(x-(2.0f*PI),ROUGHNESS);
    double ret = ((((2.0f*PI)-x) * n0) + (x * n1)) / (2.0f*PI);
    return ret;
}

double planet_getRadius(double rad)
{
    return radius + (radius * noiseLoop(rad) * NOISE_RATIO);
}

/**
 * returns the slope in radians
 */
double planet_getSlope(double rad)
{
    double dr = 5.0f / (radius);
    double p0 = noiseLoop(rad - dr) * NOISE_RATIO;
    double p1 = noiseLoop(rad + dr) * NOISE_RATIO;
    double dx = 10.0f * dr;
    double dy = p1 - p0;
    return -atan2(dy, dx);
}

void planet_rotate(double rad)
{
    rot += rad;
    TOP = 3.0f * PI / 2.0f;
}

extern double xoffset;
extern double yoffset;
extern double angle;
void planet_draw(SDL_Surface *s)
{
    radius *= (1.0f + ((easeRadius / radius)-1.0f)*0.05f);
    int rough = ROUGHNESS;
    double i;
    double r = radius;
    double dr = 0.5f / (r);
    double noiseVal;
    yoffset=sin(PI/2.0f+angle)*(planet_getRadius(TOP-angle))+(2*SCREENY/3);
    xoffset=cos(PI/2.0f+angle)*(-planet_getRadius(TOP-angle))+(SCREENX/2);
    double xStart = 0.0f;
    double xEnd = 2 * PI;

    //clip circle, so not rendering useless offscreen stuff
    if(r > SCREENX){
        double radOffset = asin(SCREENX / r);
        xStart = TOP - angle - radOffset;
        xEnd = TOP - angle + radOffset;
    }

    for(i = xStart; i < xEnd; i += dr){
        r = planet_getRadius(i);
        setPixel(s,
                (int)(xoffset + r * cos(i)),
                (int)(yoffset + r * sin(i)),
                0xFFFFFFFF);
    }
}
