#include <stdlib.h>
#include <stdio.h>

#include <cs106b/error.h>
#include <cs106b/bitvec.h>

int main(int argc, char *argv[])
{
    struct bitvec v1;
    struct bitvec v2;
    struct bitvec *v3;
    size_t i;
    int ret;
    char bits[17] = {1, 0, 1, 1, 0, 0, 0, 1,
                     0, 1, 0, 1, 1, 1, 1, 0,
                     1};

    ret = EXIT_FAILURE;

    // add bits into bitvec
    printf("bitvec_init(v1)\n");
    bitvec_init(&v1);
    for (i = 0; i < sizeof(bits); i++) {
        if (bitvec_add(&v1, bits[i]))
            goto free_v1;
        printf("bitvec_add(v1, '%i')\n", bits[i]);
    }

    // dump bitvec
    printf("bitvec_dump(v1)\n");
    if (bitvec_dump(&v1))
        goto free_v2;
    printf("\n");

    // copy bitvec
    printf("bitvec_init(v2)\n");
    bitvec_init(&v2);
    printf("bitvec_copy(v2, v1)\n");
    if (bitvec_copy(&v2, &v1))
        goto free_v2;
    printf("bitvec_dump(v2)\n");
    if (bitvec_dump(&v2))
        goto free_v2;
    printf("\n");

    // clone bitvec
    printf("bitvec_clone(v3, v1)\n");
    if (bitvec_clone(&v3, &v1))
        goto free_v2;
    printf("bitvec_dump(v3)\n");
    if (bitvec_dump(v3))
        goto free_v3;
    printf("\n");

    ret = EXIT_SUCCESS;

free_v3:
    bitvec_free(v3);
    free(v3);
free_v2:
    bitvec_free(&v2);
free_v1:
    bitvec_free(&v1);
    return ret;
}
