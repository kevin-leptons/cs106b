#include <cs106b/sorting.h>
#include <cs106b/error.h>


int sort_ins(struct vector *v, enum sort_dir dir, sort_cmp cmp)
{
    void *tmp;
    void *cursor;
    size_t i;
    size_t k;
    int shift_flag;

    if (v->size < 2)
        return 0;

    shift_flag = (dir == SORT_INC) ? -1 : 1;

    for (i = 1; i < v->size; i++) {
        if (vector_get(v, i, &tmp))
            return -1;
        for (k = i; k > 0; k--) {
            if (vector_get(v, k - 1, &cursor))
                return -1;
            if (cmp(cursor, tmp) == shift_flag) {
                if (vector_set(v, k, cursor))
                    return -1;
            } else {
                break;
            }
        }
        if (vector_set(v, k, tmp))
            return -1;
    }

    return 0;
}
