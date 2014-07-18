/**
 * list.c
 * GameJam
 * January 28, 2012
 * Brandon Surmanski
 */

#include <stdlib.h>
#include <string.h>

#include "list.h"

typedef struct Node{
    struct Node *prev;
    struct Node *next;
    void *data;
} Node;

struct List{
    size_t eleSz;
    Node *first;
    Node *last;
    Node *itter;
};

List *list_new(size_t eleSz)
{
    List *l = malloc(sizeof(List));
    l->eleSz = eleSz;
    l->first = 0;
    l->itter = 0;
}

void list_add(List *l, void *ele)
{
    Node *n = malloc(sizeof(Node));
    n->prev = 0;
    n->next = 0;
    n->data = malloc(l->eleSz);
    memcpy(n->data, ele, l->eleSz);

    if(!l->first){
        l->first = n;
        l->last = n;
    } else {
        l->last->next  = n;
        n->prev = l->last;
        l->last = n;
    }
}

void *list_itter(List *l)
{
    if(!l->itter)
        l->itter = l->first;
    else
        l->itter = l->itter->next;
    return l->itter ? l->itter->data : 0;
}

void list_removeLastItter(List *l)
{
    Node *prev = l->itter->prev;
    Node *next = l->itter->next;
    Node *n = l->itter;
    //l->itter = l->first;
    if(prev){
        prev->next = next;
        l->itter = prev;
    } else {
        l->first = next;
        if (!next){
            l->itter = 0;
        }
    }
    if(next){
        next->prev = prev;
    }
    //FIXME haha! memory leak on particles...
    //free(n->data);
    //free(n);
}

void list_clear(List *l)
{
    l->first = 0;
    l->last = 0;
    l->itter = 0;
}

void list_delete(List *l)
{
    Node *n = l->first;
    Node *next = n->next;
    while(n){
        free(n->data);
        free(n);
        n = next;
        next = n->next;
    }
    free(l);
}
