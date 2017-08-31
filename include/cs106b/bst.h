#ifndef __CS106B_BST_H__
#define __CS106B_BST_H__

#include <stdlib.h>
#include <cs106b/error.h>

struct bst
{
    void *value;
    size_t key;
    struct bst *left;
    struct bst *right;
};

int bst_add(struct bst **tree, size_t key, void *value);
int bst_del(struct bst **tree, size_t key);
size_t bst_min(struct bst *tree);
size_t bst_max(struct bst *tree);
void bst_free(struct bst **tree);

#endif
