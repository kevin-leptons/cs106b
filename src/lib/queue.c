#include <cs106b/queue.h>
#include <cs106b/error.h>
#include <cs106b/mem.h>

void queue_init(struct queue *q)
{
    q->front = NULL;
    q->back = NULL;
    q->size = 0;
}

void queue_free(struct queue *q)
{
    for (; q->size > 0; )
        queue_pop(q, NULL);
}

int queue_push(struct queue *q, void *value)
{
    struct queue_item *new_item;

    if (cs106b_malloc((void *) &new_item, sizeof(struct queue_item)))
        return -1;

    new_item->value = value;
    new_item->next = NULL;

    if (q->front == NULL)
        q->front = new_item;
    if (q->back != NULL)
        q->back->next = new_item;
    q->back = new_item;
    q->size += 1;

    return 0;
}

int queue_pop(struct queue *q, void **value)
{
    struct queue_item *front;

    if (q->front == NULL) {
        espace_raise(CS106B_EINDEX);
        return -1;
    }

    front = q->front;
    q->front = front->next;
    if (q->front == NULL)
        q->back = NULL;
    q->size -= 1;

    if (value != NULL)
        *value = front->value;
    free(front);
    return 0;
}

int queue_front(struct queue *q, void **value)
{
    if (q->front == NULL) {
        espace_raise(CS106B_EINDEX);
        return -1;
    }
    *value = q->front->value;
    return 0;
}

int queue_copy(struct queue *dest, struct queue *src)
{
    struct queue_item *item;

    queue_free(dest);
    for (item = src->front; item != NULL; item = item->next) {
        if (queue_push(dest, item->value)) {
            queue_free(dest);
            return -1;
        }
    }

    return 0;
}

int queue_clone(struct queue **dest, struct queue *src)
{
    if (cs106b_malloc((void *) dest, sizeof(struct queue)))
        return -1;
    queue_init(*dest);
    if (queue_copy(*dest, src)) {
        free(*dest);
        return -1;
    }
    return 0;
}
