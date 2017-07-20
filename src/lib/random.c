#include <cs106b/random.h>

#include <stdlib.h>
#include <time.h>

int rrand(size_t min, size_t max, size_t *result)
{
    static int _has_call_srand = 0;

    if (min > max || max > RAND_MAX)
        return -1;

    if (min == max) {
        *result = min;
        return 0;
    }

    if (!_has_call_srand) {
        srand(time(NULL));
        _has_call_srand = 1;
    }

    *result = min + (rand() % (max + 1 - min));

    return 0;
}
