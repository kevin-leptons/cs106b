/*
NAME
====

    graph - work with graph.

SYNOPSIS
========

RETURNS
=======

ERRORS
======
*/

#ifndef _CS106B_GRAPH_H_
#define _CS106B_GRAPH_H_

#include <stdlib.h>

#include <cs106b/pset.h>
#include <cs106b/htab.h>
#include <cs106b/ihtab.h>
#include <cs106b/i2htab.h>
#include <cs106b/list.h>

enum graph_type
{
    GRAPH_NONE,
    GRAPH_DIRECTED,
    GRAPH_UDIRECTED
};

struct graph_vertex
{
    size_t id;
    struct pset edges;          // pset<graph_edge *>
    size_t cost;                // cost to pass through vertex
    void *data;                 // extend data of vertex
};

struct graph_edge
{
    struct graph_vertex *begin;
    struct graph_vertex *end;
    size_t weight;              // cost to pass through edge
};

struct graph
{
    enum graph_type type;
    struct ihtab vertexes;        // htab<size_t, graph_vertex>
    struct i2htab edges;          // htab<<size_t, size_t>, graph_edge>

    struct pset _vertex_set;        // pset<graph_vertex *>
    struct pset _edge_set;          // pset<graph_edge *>
};

void graph_init(struct graph *g, enum graph_type type);
void graph_free(struct graph *g);
int graph_add_vertex(struct graph *g, size_t id, size_t cost, void *data);
int graph_add_edge(struct graph *g, size_t begin, size_t end, size_t weight);
int graph_del_vertex(struct graph *g, size_t id);
int graph_del_edge(struct graph *g, size_t begin, size_t end);
struct pset * graph_nb(struct graph *g, size_t id);

struct pset * graph_edges(struct graph *g);
struct pset * graph_edges_of(struct graph *g, size_t id);
struct graph_edge * graph_edge(struct graph *g, size_t v1, size_t v2);
struct graph_vertex * graph_vertex(struct graph *g, size_t id);
struct pset *graph_vertexes(struct graph *g);
bool graph_is_nb(struct graph *g, size_t v1, size_t v2);

int graph_dfs(struct graph *g, size_t v1, size_t v2, struct list *path);
int graph_bfs(struct graph *g, size_t v1, size_t v2, struct list *path);
int graph_djt(struct graph *g, size_t v1, size_t v2, struct list *path);
int graph_ast(struct graph *g, size_t v1, size_t v2, struct list *path);
int graph_krk(struct graph *g, struct pset *edges);

size_t graph_cost(struct graph *g, struct list *path);
int graph_dump(struct graph *g);
const char * graph_strtype(enum graph_type type);

#endif
