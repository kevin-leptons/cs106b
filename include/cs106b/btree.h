/*
SYNOPSIS
COMPLEXITY
DESCRIPTION
RETURNS
AUTHORS

    Kevin Leptons <kevin.leptons@gmail.com>
*/

#ifndef __CS106B_BTREE_H__
#define __CS106B_BTREE_H__

struct btree_node
{
    void *data;
    struct btree_node *left;
    struct btree_node *right;
};

struct btree
{
    struct btree_node *root;
};

typedef int (*btree_walkcb)(struct btree_node *node, void *arg);

int btree_init(struct btree *tree);
struct btree_node *
btree_mknode(struct btree_node *left, struct btree_node *right, void *data);
int btree_prew(struct btree *tree, btree_walkcb callback, void *arg);
int btree_inw(struct btree *tree, btree_walkcb callback, void *arg);
int btree_postw(struct btree *tree, btree_walkcb callback, void *arg);
int btree_levelw(struct btree *tree, btree_walkcb callback, void *arg);
int btree_dump(struct btree * tree);
void btree_free(struct btree *tree);

#endif
