/**
 * tree.h
 * GameJam
 * January 28, 2012
 * Brandon Surmanski
 */

typedef struct Tree {
    SDL_Surface *tree;
    float x;
    float y;
}Tree;

Tree *tree_new(float x, float y);
void tree_update(Tree *t);
void tree_draw(Tree *t)
