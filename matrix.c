/**
 * matrix.c
 * GameJam
 * January 27, 2012
 * Brandon Surmanski
 */

#include <math.h>

#include "matrix.h"
#include "planet.h"

extern void setPixel(SDL_Surface *s, int x, int y, int color);
extern int getPixel(SDL_Surface *s, int x, int y);

void matrix_rotation2(float *m, float rad)
{
    float s = sin(rad);
    float c = cos(rad);
    m[0] = c;
    m[1] = -s;
    m[2] = s;
    m[3] = c;
}

void matrix_scale2(float *m, float scale)
{
    m[0] = scale;
    m[1] = 0.0f;
    m[2] = 0.0f;
    m[3] = scale;
}

void matrix_mult2(float *a, float *b, float *dest)
{
   dest[0] = a[0] * b[0] + a[1] * b[2];
   dest[1] = a[0] * b[1] + a[1] * b[3];
   dest[2] = a[2] * b[0] + a[3] * b[2];
   dest[3] = a[2] * b[1] + a[3] * b[3];
}

void matrix_multVec2(float *m, float *v, float *vo)
{
    float tmp[3] = {v[0], v[1], v[2]};
    vo[0] = m[0]*tmp[0] + m[1]*tmp[1];
    vo[1] = m[2]*tmp[0] + m[3]*tmp[1];
}

void matrix_rotateImage(SDL_Surface *in, SDL_Surface *out, float rad, int flip)
{
    int i, j;
    float vecIn[2];
    float vecOut[2];
    float mat[4];
    if(flip)
        rad = -rad;
    matrix_rotation2(mat, rad);
    for(j = 0; j < out->h; j++){
        for(i = 0; i < out->w; i++)
        {
            vecOut[0] = i - out->w/2;
            vecOut[1] = j - out->h/2;
            if(flip)
                vecOut[0] = out->w/2 - i;

            matrix_multVec2(mat, vecOut, vecIn);
            int x = (int)i;
            setPixel(out,
                    (int)i,
                    (int)j,
                    getPixel(in, vecIn[0] + in->w/2, vecIn[1] + in->h/2));
        }
    }
}
