#include <cs106b/sorting.h>

static size_t _partition(struct vector *v, enum sortdir direction,
                         size_t start, size_t end,
                         int (*cmp_fn)(void *left, void *right))
{
    size_t left_index;
    size_t right_index;
    void *left;
    void *right;
    void *pivot;
    int match;

    pivot = vector_get(v, start);
    left_index = start + 1;
    right_index = end;
    match = (direction == SORTDIR_INC) ? 1 : -1;

    for (;;) {
        for (; left_index < right_index; left_index++) {
            left = vector_get(v, left_index);
            if (cmp_fn(left, pivot) != match)
                break;
        }
        for (; right_index > left_index; right_index--) {
            right = vector_get(v, right_index);
            if (cmp_fn(right, pivot) == match)
                break;
        }
        if (left_index == right_index)
            break;
        vector_set(v, left_index, right);
        vector_set(v, right_index, left);
    }

    left = vector_get(v, left_index);
    if (cmp_fn(pivot, left) != match) {
        vector_set(v, start, left);
        vector_set(v, left_index, pivot);
    }
    return left_index;
}

int _qui_sort(struct vector *v, enum sortdir direction,
              size_t start, size_t end,
              int (*cmp_fn)(void *left, void *right))
{
    size_t boundary;

    if (start >= end) 
        return 0;

    boundary = _partition(v, direction, start, end, cmp_fn);
    if (boundary > 0) {
        if (_qui_sort(v, direction, start, boundary - 1, cmp_fn))
            return -1;
    }
    if (_qui_sort(v, direction, boundary, end, cmp_fn))
        return -1;

    return 0;
}

int qui_sort(struct vector *v, enum sortdir direction,
             int (*cmp_fn)(void *left, void *right))
{
    return _qui_sort(v, direction, 0, v->size - 1, cmp_fn);
}
