#include <cs106b/btree.h>
#include <stdlib.h>
#include <cs106b/queue.h>
#include <cs106b/stack.h>
#include <cs106b/mem.h>

static int _btree_inw(struct btree_node *node, btree_walkcb callback,
                      void *arg);
static int _btree_postw(struct btree_node *node, btree_walkcb callback,
                        void *arg);

void btree_init(struct btree *tree)
{
    tree->root = NULL;
}

int btree_mknode(struct btree_node **new_node, struct btree_node *left,
                 struct btree_node *right, void *data)
{
    if (cs106b_malloc((void *) new_node, sizeof(*new_node)))
        return -1;
    (*new_node)->left = left;
    (*new_node)->right = right;
    (*new_node)->data = data;
    return 0;
}

int btree_prew(struct btree *tree, btree_walkcb callback, void *arg)
{
    struct stack stack;
    struct btree_node *node;
    int cb_result;
    int ret;

    ret = -1;
    stack_init(&stack);
    if (tree->root != NULL) {
        if (stack_push(&stack, tree->root))
            goto free_stack;
    }

    for (; stack.size > 0;) {
        if (stack_pop(&stack, (void *) &node))
            goto free_stack;

        // do something with this node
        cb_result = callback(node, arg);
        if (cb_result == 1)
            break;
        else if (cb_result < 0)
            goto free_stack;

        // push right and left nodes in order for continue process
        // in order: left first, right last
        if (node->right != NULL) {
            if (stack_push(&stack, node->right))
                goto free_stack;
        }
        if (node->left != NULL)  {
            if (stack_push(&stack, node->left))
                goto free_stack;
        }
    }

    ret = 0;

free_stack:
    stack_free(&stack);
    return ret;
}

int btree_inw(struct btree *tree, btree_walkcb callback, void *arg)
{
    if (tree->root == NULL)
        return 0;

    return _btree_inw(tree->root, callback, arg);
}

int btree_postw(struct btree *tree, btree_walkcb callback, void *arg)
{
    if (tree->root == NULL)
        return 0;
    return _btree_postw(tree->root, callback, arg);
}

int btree_levelw(struct btree *tree, btree_walkcb callback, void *arg)
{
    struct queue queue;
    struct btree_node *node;
    int cb_result;

    queue_init(&queue);
    if (tree->root != NULL) {
        if (queue_push(&queue, tree->root))
            goto ERROR;
    }

    for (; queue.size > 0;) {
        if (queue_pop(&queue, (void *) &node))
            goto ERROR;
        cb_result = callback(node, arg);
        if (cb_result < 0)
            goto ERROR;
        else if (cb_result == 1)
            break;

        if (node->left != NULL) {
            if (queue_push(&queue, node->left))
                goto ERROR;
        }
        if (node->right != NULL) {
            if (queue_push(&queue, node->right))
                goto ERROR;
        }
    }

    queue_free(&queue);
    return 0;

ERROR:
    queue_free(&queue);
    return -1;
}

void btree_free(struct btree *tree)
{
}

static int 
_btree_inw(struct btree_node *node, btree_walkcb callback, void *arg)
{
    int cb_result;

    if (node->left != NULL) {
        if (_btree_inw(node->left, callback, arg))
            return -1;
    }
    
    cb_result = callback(node, arg);
    if (cb_result < 0)
        return -1;
    else if (cb_result == 1)
        return 1;

    if (node->right != NULL) {
        if (_btree_inw(node->right, callback, arg))
            return -1;
    }

    return 0;
}

static int
_btree_postw(struct btree_node *node, btree_walkcb callback, void *arg)
{
    if (node->left != NULL) {
        if (_btree_postw(node->left, callback, arg))
            return -1;
    }
    if (node->right != NULL) {
        if (_btree_postw(node->right, callback, arg))
            return -1;
    }

    return callback(node, arg);
}
