#include <cs106b/sorting.h>

static int _mer(struct vector *dest, struct vector *v1, struct vector *v2,
                enum sortdir direction, 
                int (*cmp_fn)(void *left, void *right))
{
    size_t i;
    size_t k;
    size_t l;
    void *left;
    void *right;
    int lchose_flag;

    lchose_flag = direction == SORTDIR_INC ? 1 : -1;
    for (i = 0, k = 0, l = 0; i < v1->size && k < v2->size; l++) {
        left = vector_get(v1, i);
        right = vector_get(v2, k);
        if (cmp_fn(left, right) == lchose_flag) {
            vector_set(dest, l, left);
            i += 1;
        } else {
            vector_set(dest, l, right);
            k += 1;
        }
    }

    for (; i < v1->size; i++, l++)
        vector_set(dest, l, vector_get(v1, i));
    for (; k < v2->size; k++, l++)
        vector_set(dest, l, vector_get(v2, k));

    return 0;
}

int mer_sort(struct vector *v, enum sortdir direction, 
                    int (*cmp_fn)(void *left, void *right))
{
    size_t mid;
    struct vector v1;
    struct vector v2;
    size_t i;
    int ret;

    if (v->size < 2)
        return 0;
    ret = 0;

    mid = v->size / 2;
    vector_init(&v1);
    vector_init(&v2);
    for (i = 0; i < mid; i++) {
        if (vector_add(&v1, vector_get(v, i))) {
            ret = -1;
            goto clear;
        }
    }
    for (i = mid; i < v->size; i++) {
        if (vector_add(&v2, vector_get(v, i))) {
            ret = -1;
            goto clear;
        }
    }

    if (mer_sort(&v1, direction, cmp_fn)) {
        ret = -1;
        goto clear;
    }
    if (mer_sort(&v2, direction, cmp_fn)) {
        ret = -1;
        goto clear;
    }
    if (_mer(v, &v1, &v2, direction, cmp_fn)) {
        ret = -1;
        goto clear;
    }

clear:
    vector_free(&v2, false);
    vector_free(&v1, false);
    return ret;
}
