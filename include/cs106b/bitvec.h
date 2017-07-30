#ifndef __CS106B_BITVECT_H__
#define __CS106B_BITVECT_H__

#include <stdlib.h>

struct bitvec
{
    char *items;
    size_t size;
    size_t max_size;
};

int bitvec_init(struct bitvec *vector);
int bitvec_add(struct bitvec *vector, char bit);
int bitvec_del(struct bitvec *vector, size_t index);
int bitvec_at(struct bitvec *vector, size_t index);
int bitvec_dump(struct bitvec *vector);
int bitvec_copy(struct bitvec *dest, struct bitvec *src);
struct bitvec * bitvec_clone(struct bitvec *src);
int bitvec_cat(struct bitvec *dest, struct bitvec *src);
int bitvec_clear(struct bitvec *vector);
int bitvec_resize(struct bitvec *vector, size_t max_byte_size);
void bitvec_free(struct bitvec *vector);

#endif
