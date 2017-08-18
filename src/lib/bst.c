#include <cs106b/bst.h>

/*#include <cs106b/error.h>*/

static struct bst * _bst_mktree(size_t key, void *value);

int bst_add(struct bst **tree, size_t key, void *value)
{
    struct bst *new_tree;

    new_tree = _bst_mktree(key, value);
    if (new_tree == NULL)
        goto mktree_err;
    if (*tree == NULL) {
        *tree = new_tree;
    } else {
    }

    return 0;

mktree_err:
    cs106berr_push_here();
    return -1;
}
int bst_del(struct bst **tree, size_t key);
struct bst * bst_min(struct bst *tree);
struct bst * bst_max(struct bst *tree);
void bst_free(struct bst **tree);

static struct bst * _bst_mktree(size_t key, void *value)
{
    struct bst *new_tree;

    new_tree = malloc(sizeof(*new_tree));
    if (new_tree == NULL)
        goto out_of_mem;
    new_tree->key = key;
    new_tree->value = value;
    new_tree->left = NULL;
    new_tree->right = NULL;

    return new_tree;

out_of_mem:
    cs106berr_push(CS106BE_OUTMEM);
    return NULL;
}
