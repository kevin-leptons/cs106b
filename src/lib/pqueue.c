#include <cs106b/pqueue.h>

#define PQUEUE_BSIZE 16
#define PQUEUE_ESIZE 2

static int _pqueue_resize(struct pqueue *q, size_t max_size);

int pqueue_init(struct pqueue *q)
{
    q->size = 0;
    q->max_size = 0;
    q->items = NULL;

    return 0;
}

int pqueue_push(struct pqueue *q, void *data, size_t priority)
{
    size_t parent;
    size_t child;
    struct pqueue_item tmp;

    // extend memory
    if (q->max_size == 0) {
        if (_pqueue_resize(q, PQUEUE_BSIZE))
            return -1;
    }
    if (q->size == q->max_size) {
        if (_pqueue_resize(q, q->max_size * PQUEUE_ESIZE))
            return -1;
    }

    // put new data into next of last item
    q->size += 1;
    q->items[q->size].data = data;
    q->items[q->size].priority = priority;

    // resort items
    child = q->size;
    for (;;) {
        parent = child / 2;
        if (parent == 0)
            break;

        // swap
        if (q->items[child].priority < q->items[parent].priority) {
            tmp = q->items[parent];
            q->items[parent] = q->items[child];
            q->items[child] = tmp;
            child = parent;
        } else {
            break;
        }
    }

    return 0;
}

void * pqueue_pop(struct pqueue *q)
{
    void *top_data;
    size_t parent;
    size_t lchild;
    size_t rchild;
    size_t child;
    struct pqueue_item tmp;

    // empty pqueue
    if (q->size == 0)
        return NULL;

    // remove top item from storage
    top_data = q->items[1].data;
    q->items[1] = q->items[q->size];
    q->size -= 1;

    // resort items
    parent = 1;
    for (;;) {
        lchild = parent * 2;
        rchild = lchild + 1;

        // out of items to sort
        if (lchild > q->size)
            break;

        // rchild is not exist, use rchild as lchild
        if (rchild > q->size)
            rchild = lchild;

        // select minimize value of priority
        if (q->items[lchild].priority < q->items[rchild].priority)
            child = lchild;
        else
            child = rchild;

        // swap
        if (q->items[child].priority < q->items[parent].priority) {
            tmp = q->items[parent];
            q->items[parent] = q->items[child];
            q->items[child] = tmp;
            parent = child;
        } else {
            break;
        }
    }

    // resize memory
    if (q->size < (q->max_size / PQUEUE_ESIZE) && q->size > PQUEUE_BSIZE) {
        if (_pqueue_resize(q, q->max_size / PQUEUE_ESIZE))
            return NULL;
    }
    
    return top_data;
}

void * pqueue_top(struct pqueue *q)
{
    if (q->size == 0)
        return NULL;

    return q->items[1].data;
}

void pqueue_free(struct pqueue *q)
{
    if (q->items != NULL) {
        free(q->items);
        q->items = NULL;
    }
    q->size = 0;
    q->max_size = 0;
}

static int _pqueue_resize(struct pqueue *q, size_t max_size)
{
    struct pqueue_item *new_items;

    new_items = realloc(q->items, sizeof(struct pqueue_item) * max_size);
    if (new_items == NULL)
        return -1;

    free(q->items);
    q->items = new_items;
    q->max_size = max_size;

    return 0;
}
