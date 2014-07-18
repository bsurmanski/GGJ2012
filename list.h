/**
 * list.h
 * GameJam
 * January 28, 2012
 * Brandon Surmanski
 */

#ifndef _LIST_H
#define _LIST_H

#include <stddef.h>

typedef struct List List;

List *list_new(size_t eleSz);
void list_add(List *l, void *ele);
void *list_itter(List *l);
void list_removeLastItter(List *l);
void list_delete(List *l);
void list_clear(List *l);

#endif
