#include <cs106b/sorting.h>

int sort_sel(struct vector *v, enum sort_dir dir, sort_cmp cmp)
{
    void *to_swap;
    void *first;
    void *cursor;
    int chose_flag;
    size_t to_swap_index;
    size_t i;
    size_t k;

    if (v->size < 2)
        return 0;

    chose_flag = dir == SORT_INC ? 1 : -1;
    for (i = 0; i < v->size - 1; i++) {
        if (vector_get(v, i, &to_swap))
            return -1;
        for (k = i + 1; k < v->size; k++) {
            if (vector_get(v, k, &cursor))
                return -1;
            if (cmp(cursor, to_swap) == chose_flag) {
                to_swap = cursor;
                to_swap_index = k;
            }
        }
        if (to_swap_index != i) {
            if (vector_get(v, i, &first))
                return -1;
            if (vector_set(v, to_swap_index, first))
                return -1;
            if (vector_set(v, i, to_swap))
                return -1;
        }
    }
    return 0;
}
