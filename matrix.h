/**
 * matrix.h
 * GameJam
 * January 27, 2012
 * Brandon Surmanski
 */

#ifndef _MATRIX_H
#define _MATRIX_H

#include <SDL/SDL.h>

void matrix_rotation2(float *m, float rad);
void matrix_scale2(float *m, float scale);
void matrix_mult2(float *m, float *b, float *dest);
void matrix_multVec2(float *m, float *v, float *vo);
void matrix_rotateImage(SDL_Surface *in, SDL_Surface *out, float rad, int flip);

#endif
