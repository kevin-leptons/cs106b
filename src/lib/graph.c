#include <cs106b/graph.h>
#include <stdio.h>
#include <cs106b/stack.h>
#include <cs106b/queue.h>
#include <cs106b/vector.h>
#include <cs106b/pqueue.h>
#include <cs106b/mem.h>
#include <cs106b/error.h>

const char _GRAPH_DIRECTED_STR[] = "directed";
const char _GRAPH_UDIRECTED_STR[] = "undirected";
const char _GRAPH_TYPE_UNK[] = "unknown";

static 
struct graph_vertex * _graph_mk_vertex(size_t id, size_t cost, void *data);
static struct graph_edge * _graph_mk_edge(struct graph *g,
                                          struct graph_vertex *begin, 
                                          struct graph_vertex *end,
                                          size_t weight);
static int _graph_rmedges_of(struct graph_vertex *v, struct graph_vertex *nv);
static int _graph_unvis(struct graph *g, struct pset *visited,
                        size_t v, struct pset *nb);

void graph_init(struct graph *g, enum graph_type type)
{
    g->type = type;
    ihtab_init(&g->vertexes);
    i2htab_init(&g->edges);
    pset_init(&g->_vertex_set);
    pset_init(&g->_edge_set);
}

void graph_free(struct graph *g)
{
    g->type = GRAPH_NONE;
    ihtab_free(&g->vertexes);
    i2htab_free(&g->edges);
    pset_free(&g->_vertex_set);
    pset_free(&g->_edge_set);
}

int graph_add_vertex(struct graph *g, size_t id, size_t cost, void *data)
{
    struct graph_vertex *v;

    // early exist
    if (!ihtab_get(&g->vertexes, id, (void *) &v))
        return 0;

    // new vertex
    v = _graph_mk_vertex(id, cost, data);
    if (v == NULL)
        return -1;
    if (ihtab_set(&g->vertexes, id, v))
        goto ERROR;

    // add to set which index by pointer
    if (pset_add(&g->_vertex_set, v))
        goto ERROR;

    return 0;

ERROR:
    free(v);
    return -1;
}

int graph_add_edge(struct graph *g, size_t begin, size_t end, size_t weight)
{
    struct graph_vertex *v_begin;
    struct graph_vertex *v_end;
    struct graph_edge *e;

    v_begin = NULL;
    v_end = NULL;

    // add vertex: begin
    if (graph_add_vertex(g, begin, 0, NULL))
        goto ERROR;
    if (ihtab_get(&g->vertexes, begin, (void *) &v_begin))
        goto ERROR;

    // add vertex: end
    if (graph_add_vertex(g, end, 0, NULL))
        goto ERROR;
    if (ihtab_get(&g->vertexes, end, (void *) &v_end))
        goto ERROR;

    // add edge 
    if (!i2htab_get(&g->edges, begin, end, (void *) &e))
        return 0;
    e = _graph_mk_edge(g, v_begin, v_end, weight);
    if (e == NULL)
        goto ERROR;
    if (i2htab_set(&g->edges, begin, end, e))
        goto ERROR;

    // add edge to set which index by pointer
    if (pset_add(&g->_edge_set, e))
        goto ERROR;

    return 0;

ERROR:
    return -1;
}

int graph_del_vertex(struct graph *g, size_t id)
{
    struct graph_vertex *v;
    struct graph_vertex *nv;
    struct graph_edge *e;

    if (ihtab_get(&g->vertexes, id, (void *) &v))
        return -1;

    for (;;) {
        if (pset_next(&v->edges, (void *) &e))
            break;
        if (e->begin == v)
            nv = e->end;
        else
            nv = e->begin;

        // remove edges from vertex which is neigbour of this point 
        if (_graph_rmedges_of(nv, v))
            return -1;

        // remove edges from this point
        if (pset_del(&v->edges, e))
            return -1;

        // remove edges in table
        if (i2htab_del(&g->edges, e->begin->id, e->end->id))
            return -1;
    }

    // remove vertex from table
    if (ihtab_del(&g->vertexes, id))
        return -1;
    if (pset_del(&g->_vertex_set, v))
        return -1;
    pset_free(&v->edges);
    free(v);
    return 0;
}

int graph_del_edge(struct graph *g, size_t begin, size_t end)
{
    struct graph_vertex *v_begin;
    struct graph_vertex *v_end;
    struct graph_edge *e;

    if (ihtab_get(&g->vertexes, begin, (void *) &v_begin))
        return -1;
    if (ihtab_get(&g->vertexes, end, (void *) &v_end))
        return -1;
    if (i2htab_get(&g->edges, begin, end, (void *) &e))
        return -1;

    // remove edge in begin vertex
    if (pset_del(&v_begin->edges, e)) 
        return -1;

    // remove edge in end vertex
    if (pset_del(&v_end->edges, e))
        return -1;

    // remove edge in table
    if (i2htab_del(&g->edges, begin, end))
        return -1;
    if (pset_del(&g->_edge_set, e))
        return -1;
    free(e);
    return 0;
}

struct pset * graph_nb(struct graph *g, size_t id)
{
    struct pset *nb;
    struct graph_vertex *v;
    struct graph_edge *e;

    v = graph_vertex(g, id);
    if (v == NULL)
        return NULL;

    if (cs106b_malloc((void *) &nb, sizeof(*nb)))
        goto ERROR;
    pset_init(nb);

    pset_ireset(&v->edges);
    for (;;) {
        if (pset_next(&v->edges, (void *) &e))
            break;
        if (g->type == GRAPH_DIRECTED) {
            if (e->begin == v) {
                if (pset_add(nb, e->end)) 
                    goto ERROR;
            }
        } else if (g->type == GRAPH_UDIRECTED) {
            if (e->begin == v) {
                if (pset_add(nb, e->end))
                    goto ERROR;
            } else {
                if (pset_add(nb, e->begin))
                    goto ERROR;
            }
        } else {
            goto ERROR;
        }
    }

    return nb;

ERROR:
    free(nb);
    return NULL;
}

struct pset * graph_edges(struct graph *g)
{
    return &g->_edge_set;
}

struct pset * graph_edges_of(struct graph *g, size_t id)
{
    struct graph_vertex *v;

    if (ihtab_get(&g->vertexes, id, (void *) &v))
        return NULL;
    return &v->edges;
}

struct graph_edge * graph_edge(struct graph *g, size_t v1, size_t v2)
{
    struct graph_edge *e;

    i2htab_get(&g->edges, v1, v2, (void *) &e);
    if (g->type == GRAPH_UDIRECTED) {
        if (e == NULL)
            i2htab_get(&g->edges, v2, v1, (void *) &e);
    }
    return e;
}

struct graph_vertex * graph_vertex(struct graph *g, size_t id)
{
    struct graph_vertex *v;
    if (ihtab_get(&g->vertexes, id, (void *) &v)) {
        espace_clear();
        return NULL;
    }
    return v;
}

struct pset *graph_vertexes(struct graph *g)
{
    return &g->_vertex_set;
}

bool graph_is_nb(struct graph *g, size_t v1, size_t v2)
{
    if (g->type == GRAPH_UDIRECTED) {
        if (!i2htab_get(&g->edges, v1, v2, NULL))
            return true;
        if (!i2htab_get(&g->edges, v2, v1, NULL))
            return true;
        return false;
    } else if (g->type == GRAPH_DIRECTED) {
        return !i2htab_get(&g->edges, v1, v2, NULL);
    } else {
        return false;
    }
}

int graph_dfs(struct graph *g, size_t v1, size_t v2, struct list *path)
{
    struct pset visited;
    struct stack stack;
    struct graph_vertex *v_begin;
    struct graph_vertex *v;
    struct pset *nb_set;
    struct graph_vertex *nb;

    if (v1 == v2)
        return -1;

    pset_init(&visited);
    stack_init(&stack);

    v_begin = graph_vertex(g, v1);
    if (v_begin == NULL)
        return -1;
    if (pset_add(&visited, v_begin))
        return -1;
    if (stack_push(&stack, v_begin))
        return -1;

    for (; stack.size > 0;) {
        if (stack_top(&stack, (void *) &v))
            return -1;

        // found path
        if (v->id == v2)
            break;

        // add all of neigbours to stack
        nb_set = graph_nb(g, v->id);
        if (nb_set == NULL)
            return -1;

        pset_ireset(nb_set);
        for (;;) {
            if (pset_next(nb_set, (void *) &nb)) {
                if (stack_pop(&stack, NULL))
                    return -1;
                break;

            }
            if (pset_exist(&visited, nb))
                continue;
            if (pset_add(&visited, nb))
                return -1;
            if (stack_push(&stack, nb))
                return -1;
            break;
        }

        pset_free(nb_set);
    }

    // found or not
    if (v->id != v2)
        return -1;

    // revert stack to get correct path
    list_free(path);
    list_init(path);
    for (; stack.size > 0;) {
        if (stack_pop(&stack, (void *) &v))
            break;
        if (list_ins(path, v, 0))
            return -1;
    }

    stack_free(&stack);
    pset_free(&visited);
    return 0;
}

int graph_bfs(struct graph *g, size_t v1, size_t v2, struct list *path)
{
    struct queue queue;         // queue<vector<graph_vertex *>>
    struct vector *vpath;       // vector<graph_vertex *>
    struct graph_vertex *v;
    struct pset visited;        // pset<graph_vertex *>
    struct vector *new_vpath;   // vector<graph_vertex *>
    struct graph_vertex *nb;
    struct pset *nb_set;        // pset<graph_vertex *>
    size_t i;

    v = graph_vertex(g, v1);
    if (v == NULL)
        return -1;

    vpath = malloc(sizeof(*vpath));
    if (vpath == NULL)
        return -1;
    vector_init(vpath);
    if (vector_add(vpath, v))
        goto ERROR;

    queue_init(&queue);
    if (queue_push(&queue, vpath))
        goto ERROR;

    pset_init(&visited);
    for (; queue.size > 0;) {
        if (queue_pop(&queue, (void *) &vpath))
            goto ERROR;
        if (vector_end(vpath, (void *) &v))
            goto ERROR;
        if (v->id == v2)
            break;

        if (pset_add(&visited, v))
            goto ERROR;
        nb_set = graph_nb(g, v->id);
        if (nb_set == NULL)
            goto ERROR;
        pset_ireset(nb_set);
        for (;;) {
            if (pset_next(nb_set, (void *) &nb))
                break;
            if (pset_exist(&visited, nb))
                continue;
            if (vector_clone(&new_vpath, vpath))
                goto ERROR;
            if (vector_add(new_vpath, nb))
                goto ERROR;
            if (queue_push(&queue, new_vpath))
                goto ERROR;
        }
    }

    if (vector_end(vpath, (void *) &v))
        goto ERROR;
    if (v->id != v2)
        goto ERROR;

    // create path as list instead of vector
    list_free(path);
    list_init(path);
    for (i = 0; i < vpath->size; i++) {
        if (vector_get(vpath, i, (void *) &v))
            goto ERROR;
        if (list_add(path, v))
            goto ERROR;
    }

    queue_free(&queue);
    pset_free(&visited);
    return 0;

ERROR:
    if (vpath != NULL) {
        vector_free(vpath);
        free(vpath);
    }
    queue_free(&queue);
    pset_free(&visited);
    return -1;
}

int graph_djt(struct graph *g, size_t v1, size_t v2, struct list *path)
{
    struct pqueue pqueue;               // pqueue<vector<graph_vertex *>>
    struct vector *vpath;               // vector<graph_vertex *>
    struct vector *new_vpath;           // vector<graph_vertex *>
    struct graph_vertex *v;
    struct graph_vertex *nb;
    struct pset nb_set;                // pset<graph_vertex *>
    struct pset visited;                // pset<graph_vertex *>
    struct graph_edge *e;
    size_t prio;
    size_t i;

    vpath = NULL;

    // verify begin vertex
    v = graph_vertex(g, v1);
    if (v == NULL)
        goto ERROR;

    // priority queue
    pqueue_init(&pqueue, PQUEUE_INC);

    // first path with begin vertex
    if (cs106b_malloc((void *) &vpath, sizeof(*vpath)))
        goto ERROR;
    vector_init(vpath);
    if (vector_add(vpath, v))
        goto ERROR;

    // push first path into priority queue
    if (pqueue_push(&pqueue, vpath, v->cost))
        goto ERROR;

    // empty visited vertexes
    pset_init(&visited);

    // search
    pset_init(&nb_set);
    for (; pqueue.size > 0;) {
        if (pqueue_tpri(&pqueue, &prio))
            goto ERROR;
        if (pqueue_pop(&pqueue, (void *) &vpath))
            goto ERROR;
        if (vector_end(vpath, (void *) &v))
            goto ERROR;

        // found end vertex
        if (v->id == v2)
            break;

        // mark visited
        if (pset_exist(&visited, v)) {
            vector_free(vpath);
            free(vpath);
            vpath = NULL;
            continue;
        }
        if (pset_add(&visited, v))
            goto ERROR;

        // new path to neigbour vertexes
        if (_graph_unvis(g, &visited, v->id, &nb_set))
            goto ERROR;
        pset_ireset(&nb_set);
        for (;;) {
            if (pset_next(&nb_set, (void *) &nb))
                break;
            if (vector_clone(&new_vpath, vpath))
                goto ERROR;
            if (vector_add(new_vpath, nb))
                goto ERROR;
            e = graph_edge(g, v->id, nb->id);
            if (e == NULL)
                goto ERROR;
            if (pqueue_push(&pqueue, new_vpath, prio + nb->cost + e->weight))
                goto ERROR;
        }

        vector_free(vpath);
        free(vpath);
        vpath = NULL;
    }

    // found or not
    if (v->id == v2) {
        list_free(path);
        list_init(path);

        // vector<path> to list<path>
        for (i = 0; i < vpath->size; i++) {
            if (vector_get(vpath, i, (void *) &v))
                goto ERROR;
            if (list_add(path, v))
                goto ERROR;
        }
    }

    pqueue_free(&pqueue);
    if (vpath != NULL)
        vector_free(vpath);
    return 0;

ERROR:
    pqueue_free(&pqueue);
    if (vpath != NULL) {
        vector_free(vpath);
        free(vpath);
    }
    return -1;
}

int graph_ast(struct graph *g, size_t v1, size_t v2, struct list *path)
{
    return 0;
}

int graph_krk(struct graph *g, struct pset *edges)
{
    struct ihtab clusters;          // ihtab<size_t, pset *>
    struct pqueue order_edges;      // pqueue<graph_edge *>
    size_t cluster_count;
    struct pset *cluster;           // pset<graph_vertex *>
    struct graph_vertex *v;
    struct graph_edge *e;
    struct pset *v_set;             // pset<graph_vertex *>
    struct pset *e_set;             // pset<graph_edge *>
    struct pset *cluster_1;
    struct pset *cluster_2;

    // make clusters
    ihtab_init(&clusters);
    v_set = graph_vertexes(g);
    if (v_set == NULL)
        goto ERROR;
    pset_ireset(v_set);
    for (;;) {
        if (pset_next(v_set, (void *) &v))
            break;
        if (cs106b_malloc((void *) &cluster, sizeof(*cluster)))
            goto ERROR;
        pset_init(cluster);
        if (pset_add(cluster, v))
            goto ERROR;
        if (ihtab_set(&clusters, v->id, cluster))
            goto ERROR;
    }

    // queue of edges with priority
    pqueue_init(&order_edges, PQUEUE_INC);
    e_set = graph_edges(g);
    if (e_set == NULL)
        goto ERROR;
    pset_ireset(e_set);
    for (;;) {
        if (pset_next(e_set, (void *) &e))
            break;
        if (pqueue_push(&order_edges, e, e->weight))
            goto ERROR;
    }

    //chose edges of smallest spanning tree
    pset_free(edges);
    pset_init(edges);
    cluster_count = ihtab_size(&clusters);
    for (; cluster_count > 1;) {
        if (pqueue_pop(&order_edges, (void *) &e))
            goto ERROR;
        if (ihtab_get(&clusters, e->begin->id, (void *) &cluster_1))
            goto ERROR;
        if (ihtab_get(&clusters, e->end->id, (void *) &cluster_2))
            goto ERROR;
        if (cluster_1 == cluster_2)
            continue;

        // merge two clusters
        pset_ireset(cluster_2);
        for (;;) {
            if (pset_next(cluster_2, (void *) &v))
                break;
            if (pset_add(cluster_1, v))
                goto ERROR;
            if (ihtab_set(&clusters, v->id, cluster_1))
                goto ERROR;
        }
        pset_free(cluster_2);
        free(cluster_2);

        if (pset_add(edges, e))
            goto ERROR;
        cluster_count -= 1;
    }

    pset_free(cluster_1);
    free(cluster_1);
    ihtab_free(&clusters);
    pqueue_free(&order_edges);
    return 0;

ERROR:
    ihtab_free(&clusters);
    pqueue_free(&order_edges);
    return -1;
}

size_t graph_cost(struct graph *g, struct list *path)
{
    struct graph_vertex *v_begin;
    struct graph_vertex *v_end;
    struct graph_edge *e;
    size_t cost;

    list_ireset(path);
    if (list_next(path, (void *) &v_begin))
        return 0;

    cost = 0;
    for (;;) {
        if (list_next(path, (void *) &v_end))
            break;
        e = graph_edge(g, v_begin->id, v_end->id);
        if (e == NULL)
            return 0;
        cost += v_begin->cost + e->weight;
        v_begin = v_end;
    }

    return cost;
}

int graph_dump(struct graph *g)
{
    struct pset *vertexes;      // pset<graph_vertex *>
    struct graph_vertex *v;
    struct graph_edge *e;

    printf("type=%s, vertexes=%zu, edges=%zu\n",
            graph_strtype(g->type), ihtab_size(&g->vertexes), 
            i2htab_size(&g->edges));

    vertexes = graph_vertexes(g);
    if (vertexes == NULL)
        return -1;

    pset_ireset(vertexes);
    for (;;) {                                  // loop for vetexes in graph
        if (pset_next(vertexes, (void *) &v))
            break;
        printf("v=%zu, cost=%zu, data=%p\n", v->id, v->cost, v->data);

        pset_ireset(&v->edges);
        for (;;) {                              // loop for edges in vertex
            if (pset_next(&v->edges, (void *) &e))
                break;
            printf("    %3zu => %3zu, weight=%zu\n",
                    e->begin->id, e->end->id, e->weight);
        }
    }

    return 0;
}

const char * graph_strtype(enum graph_type type)
{
    if (type == GRAPH_DIRECTED)
        return _GRAPH_DIRECTED_STR;
    else if (type == GRAPH_UDIRECTED)
        return _GRAPH_UDIRECTED_STR;
    else
        return _GRAPH_TYPE_UNK;
}

static 
struct graph_vertex * _graph_mk_vertex(size_t id, size_t cost, void *data)
{
    struct graph_vertex *v;

    if (cs106b_malloc((void *) &v, sizeof(*v)))
        return NULL;

    v->id = id;
    v->cost = cost;
    v->data = data;
    pset_init(&v->edges);
    return v;
}

static struct graph_edge * _graph_mk_edge(struct graph *g,
                                          struct graph_vertex *begin, 
                                          struct graph_vertex *end,
                                          size_t weight)
{
    struct graph_edge *e;

    e = malloc(sizeof(*e));
    if (e == NULL)
        return NULL;

    e->begin = begin;
    e->end = end;
    e->weight = weight;

    // edge to begin vertex
    if (pset_add(&begin->edges, e))
        return NULL;

    if (g->type == GRAPH_UDIRECTED) {
        // edge to end vertex
        if (pset_add(&end->edges, e))
            return NULL;
    }

    return e;
}

static int _graph_rmedges_of(struct graph_vertex *v, struct graph_vertex *nv)
{
    struct graph_edge *e;

    pset_ireset(&v->edges);
    for (;;) {
        pset_next(&v->edges, (void *) &e);
        if (e->begin == nv || e->end == nv) {
            if (pset_del(&v->edges, e))
                return -1;
        }
    }


    return 0;
}

static int _graph_unvis(struct graph *g, struct pset *visited,
                        size_t v, struct pset *nb)
{
    struct pset *all_nb;
    struct graph_vertex *v_nb;

    pset_free(nb);
    pset_init(nb);

    all_nb = graph_nb(g, v);
    if (all_nb == NULL)
        return -1;

    pset_ireset(all_nb);
    for (;;) {
        if (pset_next(all_nb, (void *) &v_nb))
            break;
        if (pset_exist(visited, v_nb))
            continue;
        if (pset_add(nb, v_nb))
            return -1;
    }

    return 0;
}
