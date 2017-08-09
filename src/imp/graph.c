#include <stdio.h>
#include <stdlib.h>

#include <cs106b/error.h>
#include <cs106b/graph.h>

int main(int argc, char *argv[])
{
    struct graph g;
    struct graph_vertex *v;
    struct list path;

    if (graph_init(&g, GRAPH_DIRECTED))
        exit_err("graph_init");

    // vertexes
    if (graph_add_vertex(&g, 1, 2, NULL))
        exit_err("graph_add_vertex");
    if (graph_add_vertex(&g, 2, 8, NULL))
        exit_err("graph_add_vertex");
    if (graph_add_vertex(&g, 3, 4, NULL))
        exit_err("graph_add_vertex");
    if (graph_add_vertex(&g, 4, 1, NULL))
        exit_err("graph_add_vertex");
    if (graph_add_vertex(&g, 5, 1, NULL))
        exit_err("graph_add_vertex");
    if (graph_add_vertex(&g, 6, 1, NULL))
        exit_err("graph_add_vertex");

    // edge 1 => 2
    if (graph_add_edge(&g, 1, 2, 10))
        exit_err("graph_add_edge");

    // edge 2 => 4
    if (graph_add_edge(&g, 2, 4, 1))
        exit_err("graph_add_edge");

    // edge 4 => 1
    if (graph_add_edge(&g, 4, 1, 3))
        exit_err("graph_add_edge");

    // edge 1 => 3
    if (graph_add_edge(&g, 1, 3, 1))
        exit_err("graph_add_edge");

    // edge 3 => 1
    if (graph_add_edge(&g, 3, 1, 1))
        exit_err("graph_add_edge");

    // edge 1 => 5
    if (graph_add_edge(&g, 1, 5, 2))
        exit_err("graph_add_edge");

    // edge 5 => 6
    if (graph_add_edge(&g, 5, 6, 2))
        exit_err("graph_add_edge");

    // edge 6 => 4
    if (graph_add_edge(&g, 6, 4, 2))
        exit_err("graph_add_edge");

    // dump
    if (graph_dump(&g))
        exit_err("graph_dump");

    // verify
    if (graph_is_nb(&g, 1, 2))
        printf("1, 2 is neigbour\n");
    else
        exit_err("graph_is_nb");
    if (!graph_is_nb(&g, 2, 1))
        printf("2, 1 is not neigbour\n");
    else
        exit_err("graph_is_nb");

    printf("dfs 1 => 4\n");
    if (list_init(&path))
        exit_err("list_init");
    if (graph_dfs(&g, 1, 4, &path))
        exit_err("graph_dfs");
    list_rsiter(&path);
    for (;;) {
        v = list_next(&path);
        if (v == NULL)
            break;
        printf("%zu => ", v->id);
    }
    printf("/ edges=%zu, cost=%zu\n", path.size, graph_cost(&g, &path));

    printf("bfs 1 => 4\n");
    if (list_init(&path))
        exit_err("list_init");
    if (graph_bfs(&g, 1, 4, &path))
        exit_err("graph_bfs");
    list_rsiter(&path);
    for (;;) {
        v = list_next(&path);
        if (v == NULL)
            break;
        printf("%zu => ", v->id);
    }
    printf("/ edges=%zu, cost=%zu\n", path.size, graph_cost(&g, &path));

    printf("dijkstra 1 => 4\n");
    if (list_init(&path))
        exit_err("list_init");
    if (graph_djt(&g, 1, 4, &path))
        exit_err("graph_djt");
    list_rsiter(&path);
    for (;;) {
        v = list_next(&path);
        if (v == NULL)
            break;
        printf("%zu => ", v->id);
    }
    printf("/ edges=%zu, cost=%zu\n", path.size, graph_cost(&g, &path));


    return EXIT_SUCCESS;
}
