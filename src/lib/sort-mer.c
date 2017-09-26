#include <cs106b/sorting.h>

static int _merge(struct vector *dest, struct vector *v1, struct vector *v2,
                enum sortdir dir, sort_cmp cmp)
{
    size_t i;
    size_t k;
    size_t l;
    void *left;
    void *right;
    int lchose_flag;

    lchose_flag = dir == SORTDIR_INC ? 1 : -1;
    for (i = 0, k = 0, l = 0; i < v1->size && k < v2->size; l++) {
        if (vector_get(v1, i, &left))
            return -1;
        if (vector_get(v2, k, &right))
            return -1;
        if (cmp(left, right) == lchose_flag) {
            if (vector_set(dest, l, left))
                return -1;
            i += 1;
        } else {
            if (vector_set(dest, l, right))
                return -1;
            k += 1;
        }
    }

    for (; i < v1->size; i++, l++) {
        if (vector_get(v1, i, &left))
            return -1;
        if (vector_set(dest, l, left))
            return -1;
    }
    for (; k < v2->size; k++, l++) {
        if (vector_get(v2, k, &right))
            return -1;
        if (vector_set(dest, l, right))
            return -1;
    }

    return 0;
}

int sort_mer(struct vector *v, enum sortdir dir, sort_cmp cmp)
{
    size_t mid;
    struct vector v1;
    struct vector v2;
    size_t i;
    int ret;
    void *tmp;

    if (v->size < 2)
        return 0;
    ret = -1;

    mid = v->size / 2;
    vector_init(&v1);
    vector_init(&v2);
    for (i = 0; i < mid; i++) {
        if (vector_get(v, i, &tmp))
            goto clear;
        if (vector_add(&v1, tmp))
            goto clear;
    }
    for (i = mid; i < v->size; i++) {
        if (vector_get(v, i, &tmp))
            goto clear;
        if (vector_add(&v2, tmp))
            goto clear;
    }

    if (sort_mer(&v1, dir, cmp))
        goto clear;
    if (sort_mer(&v2, dir, cmp))
        goto clear;
    if (_merge(v, &v1, &v2, dir, cmp))
        goto clear;

    ret = 0;

clear:
    vector_free(&v2);
    vector_free(&v1);
    return ret;
}
