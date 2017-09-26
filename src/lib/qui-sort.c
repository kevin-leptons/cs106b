#include <cs106b/sorting.h>

static size_t _partition(struct vector *v, enum sortdir dir,
                         size_t start, size_t end, sort_cmp cmp)
{
    size_t left_index;
    size_t right_index;
    void *left;
    void *right;
    void *pivot;
    int match;

    if (vector_get(v, start, &pivot))
        return -1;
    left_index = start + 1;
    right_index = end;
    match = (dir == SORTDIR_INC) ? 1 : -1;

    for (;;) {
        for (; left_index < right_index; left_index++) {
            if (vector_get(v, left_index, &left))
                return -1;
            if (cmp(left, pivot) != match)
                break;
        }
        for (; right_index > left_index; right_index--) {
            if (vector_get(v, right_index, &right))
                return -1;
            if (cmp(right, pivot) == match)
                break;
        }
        if (left_index == right_index)
            break;
        if (vector_set(v, left_index, right))
            return -1;
        if (vector_set(v, right_index, left))
            return -1;
    }

    if (vector_get(v, left_index, &left))
        return -1;
    if (cmp(pivot, left) != match) {
        if (vector_set(v, start, left))
            return -1;
        if (vector_set(v, left_index, pivot))
            return -1;
    }
    return left_index;
}

int _qui_sort(struct vector *v, enum sortdir dir,
              size_t start, size_t end, sort_cmp cmp)
{
    size_t boundary;

    if (start >= end) 
        return 0;

    boundary = _partition(v, dir, start, end, cmp);
    if (boundary > 0) {
        if (_qui_sort(v, dir, start, boundary - 1, cmp))
            return -1;
    }
    if (_qui_sort(v, dir, boundary, end, cmp))
        return -1;

    return 0;
}

int sort_qui(struct vector *v, enum sortdir dir, sort_cmp cmp)
{
    return _qui_sort(v, dir, 0, v->size - 1, cmp);
}
