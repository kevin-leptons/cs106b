#include <cs106b/sorting.h>
#include <cs106b/error.h>


int ins_sort(struct vector *v, enum sortdir direction,
             int (*cmp_fn)(void *left, void *right))
{
    void *tmp;
    void *cursor;
    size_t i;
    size_t k;
    int must_shift;

    if (v->size < 2)
        return 0;

    must_shift = direction == SORTDIR_INC ? -1 : 1;

    for (i = 1; i < v->size; i++) {
        tmp = vector_get(v, i);
        for (k = i; k > 0; k--) {
            cursor = vector_get(v, k - 1);
            if (cmp_fn(cursor, tmp) == must_shift)
                vector_set(v, k, cursor);
            else
                break;
        }
        vector_set(v, k, tmp);
    }

    return 0;
}
