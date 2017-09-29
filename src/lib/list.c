#include <cs106b/list.h>
#include <cs106b/mem.h>
#include <cs106b/error.h>

static int _list_get(struct list *list, size_t index, struct list_item **item);

void list_init(struct list *list)
{
    list->front = NULL;
    list->back = NULL;
    list->size = 0;
    list->iter = NULL;
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

int list_add(struct list *list, void *value)
{
    struct list_item *new_item;

    if (cs106b_malloc((void *) &new_item, sizeof(*new_item)))
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

    if (cs106b_malloc((void *) &new_item, sizeof(*new_item)))
        return -1;
    new_item->prev = NULL;
    new_item->next = NULL;
    new_item->value = value;

    if (!_list_get(list, index, &item)) {
        if (espace_catch(CS106B_EINDEX)) {
            new_item->next = item;
            new_item->prev = item->prev;
            if (item->prev != NULL)
                item->prev->next = new_item;
            item->prev = new_item;
        } else {
            return -1;
        }
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

    if (_list_get(list, index, &item))
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

int list_get(struct list *list, size_t index, size_t **value)
{
    struct list_item *item;

    if (_list_get(list, index, &item))
        return -1;
    *value = item->value;
    return 0;
}

int list_next(struct list *list, void **value)
{
    if (list->iter == NULL) {
        espace_raise(CS106B_EINDEX);
        return -1;
    }
    *value = list->iter->value;
    list->iter = list->iter->next;
    return 0;
}

void list_ireset(struct list *list)
{
    list->iter = list->front;
}

static int _list_get(struct list *list, size_t index, struct list_item **item)
{
    size_t i;

    *item = list->front;
    for (i = 0; *item != NULL; *item = (*item)->next, i++)
        if (i == index)
            return 0;
    espace_raise(CS106B_EINDEX);
    return -1;
}
