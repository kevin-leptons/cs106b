#include <cs106b/bst.h>

#include <cs106b/error.h>

static struct bst * _bst_mktree(size_t key, void *value);
static struct bst * _bst_lookup(struct bst *tree, size_t key,
                                struct bst ***in_parent);

int bst_add(struct bst **tree, size_t key, void *value)
{
    struct bst *new_tree;
    struct bst *t;

    new_tree = _bst_mktree(key, value);
    if (new_tree == NULL)
        goto mktree_err;

    if (*tree == NULL) {
        *tree = new_tree;
        return 0;
    }

    t = *tree;
    for (;;) {
        if (key <= t->key) {
            if (t->left != NULL) {
                t = t->left;
                continue;
            }
            t->left = new_tree;
            break;
        } else {
            if (t->right != NULL) {
                t = t->right;
                continue;
            }
            t->right = new_tree;
            break;
        }
    }

    return 0;

mktree_err:
    cs106berr_emit();
    return -1;
}

int bst_del(struct bst **tree, size_t key)
{
    struct bst *target;
    struct bst **in_parent;

    target = _bst_lookup(*tree, key, &in_parent);
    if (target == NULL)
        return -1;

    for (;;) {
        // leaf node
        if (target->left == NULL && target->right == NULL) {
            if (in_parent == NULL)
                *tree = NULL;
            else
                *in_parent = NULL;
            free(target);
            break;
        }

        // both left and right child is exist
        if (target->left != NULL && target->right != NULL) {
            target->key = target->right->key;
            target->value = target->right->value;
            in_parent = &target->right;
            target = target->right;
            continue;
        }

        // only left child is exist
        if (target->left != NULL && target->right == NULL) {
            if (in_parent == NULL)
                *tree = target->left;
            else
                *in_parent = target->left;
            free(target);
            break;
        }

        // only right child is exist
        if (target->right != NULL && target->left == NULL) {
            if (in_parent == NULL)
                *tree = target->right;
            else
                *in_parent = target->right;
            free(target);
            break;
        }
    }


    return 0;
}

size_t bst_min(struct bst *tree)
{
    struct bst *target;

    for (target = tree; ;) {
        if (target == NULL)
            break;
        if (target->left == NULL)
            break;
        target = target->left;
    }

    if (target == NULL)
        return 0;
    else
        return target->key;
}

size_t bst_max(struct bst *tree)
{
    struct bst *target;

    for (target = tree; ;) {
        if (target == NULL)
            break;
        if (target->right == NULL)
            break;
        target = target->right;
    }

    if (target == NULL)
        return 0;
    else
        return target->key;
}

void bst_free(struct bst **tree)
{
    if ((*tree)->left != NULL) {
        bst_free(&(*tree)->left);
        (*tree)->left = NULL;
    }
    if ((*tree)->right != NULL) {
        bst_free(&(*tree)->right);
        (*tree)->right = NULL;
    }
    free(*tree);
    *tree = NULL;
}

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

static struct bst * _bst_lookup(struct bst *tree, size_t key,
                                struct bst ***in_parent)
{
    struct bst *target;

    *in_parent = NULL;
    for (target = tree, *in_parent = NULL;;) {
        if (target->key == key) {
            return target;
        } else if (target->key < key) {
            *in_parent = &target->right;
            target = target->right;
        } else {
            *in_parent = &target->left;
            target= target->left;
        }
    }

    return NULL;
}
