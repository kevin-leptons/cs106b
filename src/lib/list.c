#include <cs106b/list.h>

static struct list_item * _list_at(struct list *list, size_t index);

int list_init(struct list *list)
{
    list->front = NULL;
    list->back = NULL;
    list->size = 0;
    list->iter = NULL;

    return 0;
}

int list_add(struct list *list, void *value)
{
    struct list_item *new_item;

    new_item = malloc(sizeof(*new_item));
    if (new_item == NULL)
        return -1;
    new_item->next = NULL;
    new_item->prev = NULL;
    new_item->value = value;

    if (list->back != NULL) {
        list->back->next = new_item;
        new_item->prev = list->back;
    } else {
        list->front = new_item;
    }

    list->back = new_item;
    list->size += 1;
    return 0;
}

int list_ins(struct list *list, void *value, size_t index)
{
    struct list_item *item;
    struct list_item *new_item;

    new_item = malloc(sizeof(*new_item));
    if (new_item == NULL)
        return -1;
    new_item->prev = NULL;
    new_item->next = NULL;
    new_item->value = value;

    item = _list_at(list, index);
    if (item != NULL) {
        new_item->next = item;
        new_item->prev = item->prev;
        if (item->prev != NULL)
            item->prev->next = new_item;
        item->prev = new_item;
    }

    if (new_item->prev == NULL)
        list->front = new_item;
    if (new_item->next == NULL)
        list->back = new_item;
    list->size += 1;

    return 0;
}

int list_del(struct list *list, size_t index)
{
    struct list_item *item;

    item = _list_at(list, index);
    if (item == NULL)
        return -1;

    if (item->prev != NULL)
        item->prev->next = item->next;
    else
        list->front = item->next;

    if (item->next != NULL)
        item->next->prev = item->prev;
    else
        list->back = item->prev;
    
    free(item);
    list->size -= 1;
    return 0;
}

void * list_at(struct list *list, size_t index)
{
    struct list_item *item;

    item = _list_at(list, index);
    if (item == NULL)
        return NULL;

    return item->value;
}

void * list_next(struct list *list)
{
    void *value;

    if (list->iter == NULL)
        return NULL;
    value = list->iter->value;
    list->iter = list->iter->next;

    return value;
}

void list_rsiter(struct list *list)
{
    list->iter = list->front;
}

void list_free(struct list *list)
{
    struct list_item *item;
    struct list_item *tmp;

    for (item = list->front; item != NULL;) {
        tmp = item->next;
        free(item);
        item = tmp;
    }

    list->front = NULL;
    list->back = 0;
    list->size = 0;
}

static struct list_item * _list_at(struct list *list, size_t index)
{
    size_t i;
    struct list_item *item;

    item = list->front;
    for (item = list->front, i = 0; item != NULL; item = item->next, i++)
        if (i == index)
            break;

    return item;
}
