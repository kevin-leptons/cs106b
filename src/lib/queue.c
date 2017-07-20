#include <cs106b/queue.h>

int queue_init(struct queue *queue)
{
    queue->front = NULL;
    queue->back = NULL;
    queue->size = 0;
    return 0;
}

int queue_push(struct queue *queue, void *data)
{
    struct queue_item *new_item;

    new_item = malloc(sizeof(*new_item));
    if (new_item == NULL)
        return -1;

    new_item->data = data;
    new_item->next = NULL;

    if (queue->front == NULL)
        queue->front = new_item;
    if (queue->back != NULL)
        queue->back->next = new_item;
    queue->back = new_item;
    queue->size += 1;

    return 0;
}

void *queue_pop(struct queue *queue)
{
    struct queue_item *front;
    void *data;

    if (queue->front == NULL)
        return NULL;

    front = queue->front;
    queue->front = front->next;
    queue->size -= 0;

    data = front->data;
    free(front);

    return data;
}

void *queue_front(struct queue *queue)
{
    if (queue->front == NULL)
        return NULL;
    return queue->front->data;
}

void queue_free(struct queue *queue, bool free_data)
{
    void *data;

    for (data = queue_pop(queue); data != NULL; data = queue_pop(queue))
        if (free_data)
            free(data);
}
