#ifndef __CS106B_LIST_H__
#define __CS106B_LIST_H__

#include <stdlib.h>

struct list_item
{
    struct list_item *next;
    struct list_item *prev;
    void *value;
};

struct list
{
    struct list_item *front;
    struct list_item *back;
    size_t size;
    struct list_item *iter;
};

int list_init(struct list *list);
int list_add(struct list *list, void *value);
int list_ins(struct list *list, void *value, size_t index);
int list_del(struct list *list, size_t index);
void * list_at(struct list *list, size_t index);
void * list_next(struct list *list);
void list_rsiter(struct list *list);
void list_free(struct list *list);

#endif
