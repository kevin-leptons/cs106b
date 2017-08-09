#include <cs106b/pqueue.h>

#include <string.h>
#include <stdbool.h>

#define PQUEUE_BSIZE 16
#define PQUEUE_ESIZE 2

static int _pqueue_resize(struct pqueue *q, size_t max_size);
static int _pqueue_extend(struct pqueue *q);
static int _pqueue_narrow(struct pqueue *q);
static void _pqueue_swap(struct pqueue *q, size_t first, size_t second);

int pqueue_init(struct pqueue *q, enum pqueue_type type)
{
    q->size = 0;
    q->max_size = 0;
    q->items = NULL;
    q->type = type;

    return 0;
}

int pqueue_push(struct pqueue *q, void *data, size_t priority)
{
    size_t parent_index;
    size_t child_index;
    struct pqueue_item *child;
    struct pqueue_item *parent;

    if (_pqueue_extend(q))
        return -1;

    // put new item into next of last item
    q->size += 1;
    q->items[q->size].data = data;
    q->items[q->size].priority = priority;

    // resort items
    child_index = q->size;
    for (;;) {
        parent_index = child_index / 2;
        if (parent_index == 0)
            break;
        parent = q->items + parent_index;
        child = q->items + child_index;

        // swap
        if (q->type == PQUEUE_SMALLEST) {
            if (child->priority < parent->priority)
                _pqueue_swap(q, parent_index, child_index);
            else
                break;
        } else if (q->type == PQUEUE_BIGGEST) {
            if (child->priority > parent->priority)
                _pqueue_swap(q, parent_index, child_index);
            else
                break;
        } else {
            return -1;
        }
    }

    return 0;
}

void * pqueue_pop(struct pqueue *q)
{
    void *top_data;
    size_t parent_index;
    size_t lchild_index;
    size_t rchild_index;
    size_t child_index;
    struct pqueue_item *parent;
    struct pqueue_item *lchild;
    struct pqueue_item *rchild;
    struct pqueue_item *child;

    // empty pqueue
    if (q->size == 0)
        return NULL;

    // remove top item from storage
    top_data = q->items[1].data;
    q->items[1] = q->items[q->size];
    q->size -= 1;

    // resort items
    parent_index = 1;
    for (;;) {
        parent = q->items + parent_index;
        lchild_index = 2 * parent_index;
        rchild_index = lchild_index + 1;

        // done
        if (lchild_index > q->size)
            break;

        // rchild is not exist, use rchild as lchild
        if (rchild_index > q->size)
            rchild_index = lchild_index;

        // select child to swap
        lchild = q->items + lchild_index;
        rchild = q->items + rchild_index; 
        if (q->type == PQUEUE_SMALLEST) {
            if (lchild->priority < rchild->priority)
                child_index = lchild_index;
            else
                child_index = rchild_index;
        } else if (q->type == PQUEUE_BIGGEST) {
            if (lchild->priority > rchild->priority)
                child_index = lchild_index;
            else
                child_index = rchild_index;
        } else {
            return NULL;
        }

        // swap
        child = q->items + child_index;
        if (q->type == PQUEUE_SMALLEST) {
            if (child->priority < parent->priority)
                _pqueue_swap(q, child_index, parent_index);
            else
                break;
        } else if (q->type == PQUEUE_BIGGEST) {
            if (child->priority > parent->priority)
                _pqueue_swap(q, child_index, parent_index);
            else
                break;
        } else {
            return NULL;
        }

        parent_index = child_index;
    }

    if (_pqueue_narrow(q))
        return NULL;

    return top_data;
}

void * pqueue_top(struct pqueue *q)
{
    if (q->size == 0)
        return NULL;

    return q->items[1].data;
}

size_t pqueue_tpri(struct pqueue *q)
{
    if (q->size == 0)
        return 0;
    return q->items[1].priority;
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
    size_t new_msize;
    size_t copy_size;
    
    new_msize = sizeof(*new_items) * (max_size + 1);
    new_items = malloc(new_msize);
    if (new_items == NULL)
        return -1;
    memset(new_items, 0, new_msize);

    if (q->items != NULL) {
        copy_size = max_size < q->size ? max_size + 1: q->size + 1;
        memcpy(new_items, q->items, sizeof(*new_items) * copy_size);
        free(q->items);
    }

    q->items = new_items;
    q->max_size = max_size;

    return 0;
}

static int _pqueue_extend(struct pqueue *q)
{
    if (q->max_size == 0) {
        if (_pqueue_resize(q, PQUEUE_BSIZE))
            return -1;
    }
    if (q->size == (q->max_size - 1)) {
        if (_pqueue_resize(q, q->max_size * PQUEUE_ESIZE))
            return -1;
    }

    return 0;
}

static int _pqueue_narrow(struct pqueue *q)
{
    size_t border_bot;

    border_bot = q->max_size / PQUEUE_ESIZE;
    if (q->size < border_bot && q->max_size > PQUEUE_BSIZE) {
        if (_pqueue_resize(q, border_bot))
            return -1;
    }

    return 0;
}

static void _pqueue_swap(struct pqueue *q, size_t first, size_t second)
{
    struct pqueue_item tmp;

    tmp = q->items[first];
    q->items[first] = q->items[second];
    q->items[second] = tmp;
}
