#include <cs106b/sorting.h>

int sel_sort(struct vector *v, enum sortdir direction,
             int (*cmp_fn)(void *left, void *right))
{
    void *to_swap;
    void *first;
    void *cursor;
    int must_chose;
    size_t to_swap_index;
    size_t i;
    size_t k;

    if (v->size < 2)
        return 0;

    must_chose = direction == SORTDIR_INC ? 1 : -1;
    for (i = 0; i < v->size - 1; i++) {
        to_swap = vector_get(v, i) ;
        for (k = i + 1; k < v->size; k++) {
            cursor = vector_get(v, k);
            if (cmp_fn(to_swap, cursor) == must_chose) {
                to_swap = cursor;
                to_swap_index = k;
            }
        }
        if (to_swap_index != i) {
            first = vector_get(v, i);
            vector_set(v, to_swap_index, first);
            vector_set(v, i, to_swap);
        }
    }
    return 0;
}
