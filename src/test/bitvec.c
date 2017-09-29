#include <stdlib.h>
#include <stdio.h>

#include <cs106b/error.h>
#include <cs106b/bitvec.h>

int main(int argc, char *argv[])
{
    struct bitvec v1;
    struct bitvec v2;
    size_t i;
    char bits[17] = {1, 0, 1, 1, 0, 0, 0, 1,
                     0, 1, 0, 1, 1, 1, 1, 0,
                     1};

    bitvec_init(&v1);

    for (i = 0; i < sizeof(bits); i++) {
        if (bitvec_add(&v1, bits[i]))
            exit_err("bitvec_add");
    }

    bitvec_init(&v2);
    if (bitvec_copy(&v2, &v1))
        exit_err("bitvec_copy");

    if (bitvec_dump(&v1))
        exit_err("bitvec_dump");
    printf("\n");
    if (bitvec_dump(&v2))
        exit_err("bitvec_dump");

    bitvec_free(&v1);
    bitvec_free(&v2);
    return 0;
}
