#include <cs106b/pqueue.h>
#include <string.h>
#include <stdbool.h>
#include <cs106b/error.h>
#include <cs106b/mem.h>

#define PQUEUE_BSIZE 16
#define PQUEUE_ESIZE 2

static int _pqueue_resize(struct pqueue *q, size_t max_size);
static int _pqueue_extend(struct pqueue *q);
static int _pqueue_narrow(struct pqueue *q);
static void _pqueue_swap(struct pqueue *q, size_t first, size_t second);

void pqueue_init(struct pqueue *q, enum pqueue_dir direction)
{
    q->size = 0;
    q->max_size = 0;
    q->items = NULL;
    q->dir = direction;
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
        if (q->dir == PQUEUE_INC) {
            if (child->priority < parent->priority)
                _pqueue_swap(q, parent_index, child_index);
            else
                break;
        } else if (q->dir == PQUEUE_DEC) {
            if (child->priority > parent->priority)
                _pqueue_swap(q, parent_index, child_index);
            else
                break;
        }
        child_index = parent_index;
    }

    return 0;
}

int pqueue_pop(struct pqueue *q, void **data)
{
    size_t parent_index;
    size_t lchild_index;
    size_t rchild_index;
    size_t child_index;
    struct pqueue_item *parent;
    struct pqueue_item *lchild;
    struct pqueue_item *rchild;
    struct pqueue_item *child;

    // empty pqueue
    if (q->size == 0) {
        espace_raise(CS106B_EINDEX);
        return -1;
    }

    // remove top item from storage
    *data = q->items[1].data;
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
        if (q->dir == PQUEUE_INC) {
            if (lchild->priority < rchild->priority)
                child_index = lchild_index;
            else
                child_index = rchild_index;
        } else if (q->dir == PQUEUE_DEC) {
            if (lchild->priority > rchild->priority)
                child_index = lchild_index;
            else
                child_index = rchild_index;
        } 

        // swap
        child = q->items + child_index;
        if (q->dir == PQUEUE_INC) {
            if (child->priority < parent->priority)
                _pqueue_swap(q, child_index, parent_index);
            else
                break;
        } else if (q->dir == PQUEUE_DEC) {
            if (child->priority > parent->priority)
                _pqueue_swap(q, child_index, parent_index);
            else
                break;
        }
        parent_index = child_index;
    }

    if (_pqueue_narrow(q))
        return -1;
    return 0;
}

int pqueue_top(struct pqueue *q, void **data)
{
    if (q->size == 0) {
        espace_raise(CS106B_EINDEX);
        return -1;
    }

    *data = q->items[1].data;
    return 0;
}

int pqueue_tpri(struct pqueue *q, size_t *priority)
{
    if (q->size == 0) {
        espace_raise(CS106B_EINDEX);
        return -1;
    }
    *priority = q->items[1].priority;
    return 0;
}

int pqueue_copy(struct pqueue *dest, struct pqueue *src)
{
    size_t mem_size;

    pqueue_free(dest);
    mem_size = sizeof(struct pqueue_item) * src->max_size + 1;
    if (cs106b_malloc((void *) &dest->items, mem_size)) 
        return -1;
    memcpy(dest->items, src->items, mem_size);
    dest->size = src->size;
    dest->max_size = src->max_size;
    dest->dir = src->dir;
    return 0;
}

int pqueue_clone(struct pqueue **dest, struct pqueue *src)
{
    if (cs106b_malloc((void *) dest, sizeof(struct pqueue)))
        return -1;
    pqueue_init(*dest, src->dir);
    if (pqueue_copy(*dest, src)) {
        free(*dest);
        return -1;
    }
    return 0;
}

static int _pqueue_resize(struct pqueue *q, size_t max_size)
{
    struct pqueue_item *new_items;
    size_t new_msize;
    
    new_msize = sizeof(*new_items) * (max_size + 1);
    if (cs106b_malloc((void *) &new_items, new_msize))
        return -1;
    memset(new_items, 0, new_msize);

    if (q->items != NULL) {
        q->size = max_size < q->size ? max_size + 1: q->size + 1;
        memcpy(new_items, q->items, sizeof(*new_items) * q->size);
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
