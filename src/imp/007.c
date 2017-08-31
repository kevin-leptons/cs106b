#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#include <GL/glut.h>

#include <cs106b/error.h>
#include <cs106b/graph.h>
#include <cs106b/random.h>
#include <cs106b/list.h>

enum alg_type
{
    ALG_DFS,
    ALG_BFS,
    ALG_DJT
};

enum point2d_type
{
    POINT2D_WALL,
    POINT2D_WAY
};

struct point2d
{
    size_t x;
    size_t y;
    enum point2d_type type;
    bool in_path;
    size_t height;
};

struct point2d_map
{
    struct point2d *cells;
    size_t row_size;
    size_t col_size;
};

struct cell
{
    size_t row;
    size_t col;
};

struct pixel_ctx
{
    GLint screen_w;
    GLint screen_h;
    GLint screen_wh;
    GLint screen_hh;
    GLint screen_x;
    GLint screen_y;
    GLint cell_w;
};

struct rgb
{
    GLclampf red;
    GLclampf green;
    GLclampf blue;
    GLclampf alpha;
};

static const struct rgb COLOR_GREEN = {0.0f, 1.0f, 0.0f, 1.0f};
static const struct rgb COLOR_WHITE = {1.0f, 1.0f, 1.0f, 1.0f};

static int _point2d_map_init(struct point2d_map *map,
                             size_t row_size, size_t col_size);
static int _rand_map(struct point2d_map *map, struct graph *g,
                     size_t row_size, size_t col_size);
static int _map_link_vertex(struct point2d_map *map, struct graph *g,
                            size_t row, size_t col);

static void _render_search_hand(void);
static int _render_map(struct point2d_map *map);
static int
_render_cell(struct pixel_ctx *ctx, GLint x, GLint y, GLint w,
             struct rgb color);
static void _mouse_search_hand(int button, int state, int x, int y);
static int _pixel_to_cell(struct pixel_ctx *ctx, GLint x, GLint y,
                          struct cell *cell);
static int _cell_to_gl(struct pixel_ctx *ctx, size_t row, size_t col,
                       double *x, double *y);

static int _rand_maze_nwall(struct point2d_map *map, struct graph *g,
                              size_t row_size, size_t col_size);
static void _mouse_reduce_hand(int button, int state, int x, int y);
static void _render_reduce_hand(void);
static int _render_edges(struct pixel_ctx *ctx, struct pset *edges,
                         const struct rgb *color);

static void _cli_help(void);

static size_t _col_size;
static size_t _row_size;
static enum alg_type _alg;
static struct graph _g;
static struct point2d_map _map;
static struct pixel_ctx _ctx;
static struct cell _cell_1;
static struct cell _cell_2;
static size_t _cell_count = 0;
static bool _running = false;
static struct list _path;                       // list<graph_edge *>
static bool _krk_done = false;
static struct pset _krk_edges;                  // pset<graph_edge *>

int main(int argc, char *argv[])
{
    int opt;
    void (*render_hand)(void);
    void (*mouse_hand)(int button, int state, int x, int y);

    for (;;) {
        opt = getopt(argc, argv, "h");
        if (opt < 0)
            break;
        if (opt == 'h') {
            _cli_help();
            return EXIT_SUCCESS;
        } else {
            _cli_help();
            return EXIT_FAILURE;
        }
    }

    if (argc < 3) {
        _cli_help();
        return EXIT_FAILURE;
    }

    // size of map
    if (strcmp(argv[2], "tini") == 0) {
        _row_size = 4;
        _col_size = 4;
    } else if (strcmp(argv[2], "small") == 0) {
        _row_size = 16;
        _col_size = 16;
    } else if (strcmp(argv[2], "medium") == 0) {
        _row_size = 32;
        _col_size = 32;
    } else if (strcmp(argv[2], "large") == 0) {
        _row_size = 64;
        _col_size = 64;
    } else {
        _cli_help();
        return EXIT_FAILURE;
    }

    if (strcmp(argv[1], "search") == 0) {
        // algorithm
        if (strcmp(argv[3], "dfs") == 0) {
            _alg = ALG_DFS;
        } else if (strcmp(argv[3], "bfs") == 0) {
            _alg = ALG_BFS;
        } else if (strcmp(argv[3], "djt") == 0) {
            _alg = ALG_DJT;
        } else {
            _cli_help();
            return EXIT_FAILURE;
        }

        // random map
        if (_rand_map(&_map, &_g, _row_size, _col_size))
            exit_err("_rand_map");
        if (list_init(&_path))
            exit_err("list_init");

        // event handler
        render_hand = _render_search_hand;
        mouse_hand = _mouse_search_hand;
    } else if (strcmp(argv[1], "reduce") == 0) {
        // random maze
        if (_rand_maze_nwall(&_map, &_g, _row_size, _col_size))
            exit_err("_rand_maze_no_wall");
        if (pset_init(&_krk_edges))
            exit_err("pset_init");

        // event handlers
        render_hand = _render_reduce_hand;
        mouse_hand = _mouse_reduce_hand;
    } else {
        _cli_help();
        return EXIT_FAILURE;
    }

    // use graphical window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(800, 600);
    glutCreateWindow("cs106b.007.trailblazer");

    glutDisplayFunc(render_hand);
    glutMouseFunc(mouse_hand);
    glutMainLoop();

    return EXIT_SUCCESS;
}

static int _rand_map(struct point2d_map *map, struct graph *g,
                     size_t row_size, size_t col_size)
{
    size_t row;
    size_t col;
    size_t map_index;
    struct point2d *p;
    size_t rand_num;

    if (graph_init(g, GRAPH_UDIRECTED))
        return -1;
    if (_point2d_map_init(map, row_size, col_size))
        return -1;

    // random vertexes
    for (row = 0; row < row_size; row++) {
        for (col = 0; col < col_size; col++) {
            map_index = row * col_size + col;
            p = map->cells + map_index;
            p->x = col;
            p->y = row;
            p->in_path = false;

            if (rrand(0, 1, &rand_num))
                goto ERROR;
            if (rand_num == 0) {
                p->type = POINT2D_WALL;
                p->height = 0;
            } else if (rand_num == 1) {
                p->type = POINT2D_WAY;
                p->height = 0;
            } 

            if (graph_add_vertex(g, map_index, 0, p))
                goto ERROR;
        }
    }

    // create edges
    for (row = 0; row < row_size; row++) {
        for (col = 0; col < col_size; col++) {
            if (_map_link_vertex(map, g, row, col))
                goto ERROR;
        }
    }

    return 0;

ERROR:
    return -1;
}

static int _map_link_vertex(struct point2d_map *map, struct graph *g,
                            size_t row, size_t col)
{
    bool has_top;
    bool has_bot;
    bool has_left;
    bool has_right;
    struct point2d *p;
    struct point2d *np;
    size_t map_index;
    size_t np_index;

    map_index = row * map->col_size + col;
    p = map->cells + map_index;
    if (p->type == POINT2D_WALL)
        return 0;

    has_top = row > 0 ? true : false;
    has_bot = row < (map->row_size - 1) ? true : false;
    has_left = col > 0 ? true : false;
    has_right = col < (map->col_size - 1) ? true : false;

    if (has_top) {
        np_index = (row - 1) * map->col_size + col;
        np = map->cells + np_index;
        if (np->type != POINT2D_WALL) {
            if (graph_add_edge(g, map_index, np_index, 1))
                return -1;
        }
    }
    if (has_right) {
        np_index = row * map->col_size + col + 1;
        np = map->cells + np_index;
        if (np->type != POINT2D_WALL) {
            if (graph_add_edge(g, map_index, np_index, 1))
                return -1;
        }
    }
    if (has_bot) {
        np_index = (row + 1) * map->col_size + col;
        np = map->cells + np_index;
        if (np->type != POINT2D_WALL) {
            if (graph_add_edge(g, map_index, np_index, 1)) 
                return -1;
        }
    }
    if (has_left) {
        np_index = row * map->col_size + col - 1;
        np = map->cells + np_index;
        if (np->type != POINT2D_WALL) {
            if (graph_add_edge(g, map_index, np_index, 1))
                return -1;
        }
    }

    return 0;
}

static int _point2d_map_init(struct point2d_map *map,
                             size_t row_size, size_t col_size)
{
    size_t map_size;

    map_size = sizeof(*map->cells) * row_size * col_size;
    map->cells = malloc(map_size);
    if (map == NULL)
        return -1;
    memset(map->cells, 0, map_size);
    map->row_size = row_size;
    map->col_size = col_size;

    return 0;
}

static void _render_search_hand(void)
{
    double gl_x;
    double gl_y;
    struct graph_vertex *v;
    struct point2d *cell;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (_render_map(&_map))
        exit_err("_render_map");

    // render path line
    list_rsiter(&_path);
    glColor3f(1.0f, 0.0f, 0.0f);
    glLineWidth(2);
    glBegin(GL_LINE_STRIP);
    for (;;) {
        v = list_next(&_path);
        if (v == NULL)
            break;
        cell = v->data;
        cell->in_path = true;
        if (_cell_to_gl(&_ctx, cell->y, cell->x, &gl_x, &gl_y))
            exit_err("_cell_to_gl");
        glVertex2f(gl_x, gl_y);
    }
    glEnd();

    glutSwapBuffers();
}

static int _render_map(struct point2d_map *map)
{
    size_t row;
    size_t col;
    struct point2d *p;
    GLint viewport[4];
    GLint screen_w;
    GLint screen_h;
    GLint map_w;
    GLint map_h;
    GLint screen_x;
    GLint screen_y;
    struct rgb color;
    GLint x;
    GLint y;
    GLint cell_w;

    glGetIntegerv(GL_VIEWPORT, viewport);
    screen_w = viewport[2];
    screen_h = viewport[3];

    // find screen resolution
    map_w = map->col_size / map->row_size * screen_h;
    if (map_w <= screen_w) {
        map_h = screen_h;
        screen_x = (screen_w - map_w) / 2;
        screen_y = 0;
    } else {
        map_w = screen_w;
        map_h = screen_w * map->row_size / map->col_size;
        screen_x = 0;
        screen_y = (screen_h - map_h) / 2;
    }

    _ctx.screen_w = screen_w;
    _ctx.screen_h = screen_h;
    _ctx.screen_wh = screen_w / 2;
    _ctx.screen_hh = screen_h / 2;
    _ctx.screen_x = screen_x;
    _ctx.screen_y = screen_y;
    _ctx.cell_w = map_w / map->col_size;
    cell_w = _ctx.cell_w;
    color.alpha = 1.0f;
    for (row = 0; row < map->row_size; row++) {
        for (col = 0; col < map->col_size; col++) {
            p = map->cells + row * map->col_size + col;

            // chose cordinate
            x = screen_x + p->x * cell_w;
            y = screen_y + p->y * cell_w;

            // chose color
            if (p->type == POINT2D_WALL) {
                color.red = 0;
                color.green = 0;
                color.blue = 0;
            } else if (p->type == POINT2D_WAY) {
                color.red = 1.0f;
                color.green = 1.0f;
                color.blue = 1.0f;
            } else {
                return -1;
            }

            // render cell
            if (p->in_path) {
                if (_render_cell(&_ctx, x, y, cell_w, COLOR_GREEN))
                    return -1;
            } else {
                if (_render_cell(&_ctx, x, y, cell_w, color))
                    return -1;
            }
        }
    }

    return 0;
}

static int
_render_cell(struct pixel_ctx *ctx, GLint x, GLint y, GLint w,
             struct rgb color)
{
    double cx;
    double cy;
    double cx_next;
    double cy_next;

    // other edges
    cx_next = x + w;
    cy_next = y + w;

    // pixel cordinate to open gl cordinate
    cx = x - ctx->screen_wh;
    cy = ctx->screen_hh - y;
    cx_next = cx_next - ctx->screen_wh;
    cy_next = ctx->screen_hh - cy_next;
    cx = cx / ctx->screen_wh;
    cy = cy / ctx->screen_hh;
    cx_next = cx_next / ctx->screen_wh;
    cy_next = cy_next / ctx->screen_hh;

    // draw
    glColor3f(color.red, color.green, color.blue);
    glBegin(GL_POLYGON);
    glVertex2f(cx, cy);
    glVertex2f(cx_next, cy);
    glVertex2f(cx_next, cy_next);
    glVertex2f(cx, cy_next);
    glEnd();

    return 0;
}

static void _mouse_search_hand(int button, int state, int x, int y)
{
    struct cell cell_clicked;
    struct point2d *p;
    struct graph_vertex *v;
    struct point2d *cell;
    size_t v1;
    size_t v2;

    if (button != GLUT_LEFT_BUTTON || state != GLUT_DOWN || _running)
        return;
    _running = true;

    // clear prev path
    list_rsiter(&_path);
    if (_cell_count == 2) {
        for (;;) {
            v = list_next(&_path);
            if (v == NULL)
                break;
            cell = v->data;
            if (cell->x == _cell_2.col && cell->y == _cell_2.row)
                continue;
            cell->in_path = false;
        }
        list_free(&_path);
    }

    // process clicked cell
    if (_pixel_to_cell(&_ctx, x, y, &cell_clicked))
        goto ERROR;
    p = _map.cells + cell_clicked.row * _map.row_size + cell_clicked.col;
    if (p->type == POINT2D_WALL)
        goto REFRESH;

    if (_cell_count == 0) {
        _cell_2 = cell_clicked;
        _cell_count += 1;
    } else if (_cell_count == 1) {
        _cell_1 = _cell_2;
        _cell_2 = cell_clicked;
        _cell_count += 1;
    } else {
        p = _map.cells + _cell_1.row * _map.col_size + _cell_1.col;
        p->in_path = false;
        _cell_1 = _cell_2;
        _cell_2 = cell_clicked;
    }
    p = _map.cells + _cell_2.row * _map.col_size + _cell_2.col;
    p->in_path = true;

    if (_cell_count < 2)
        goto REFRESH;

    // find path
    v1 = _cell_1.row * _map.row_size + _cell_1.col;
    v2 = _cell_2.row * _map.row_size + _cell_2.col;
    if (_alg == ALG_DJT) {
        printf("searching: %zu -> %zu, Dijktra...\n", v1, v2);
        if (graph_djt(&_g, v1, v2, &_path))
            goto ERROR;
    } else if (_alg == ALG_DFS) {
        printf("searching: %zu -> %zu, DFS...\n", v1, v2);
        if (graph_dfs(&_g, v1, v2, &_path))
            goto ERROR;
    } else if (_alg == ALG_BFS) {
        printf("searching: %zu -> %zu, BFS...\n", v1, v2);
        if (graph_bfs(&_g, v1, v2, &_path))
            goto ERROR;
    }
    if (_path.size == 0) {
        printf("not found\n");
        goto REFRESH;
    }
    printf("found, cost=%zu, vertexes=%zu\n", graph_cost(&_g, &_path),
            _path.size);

    glutPostRedisplay();
    _running = false;

REFRESH:
    glutPostRedisplay();
    _running = false;
    return;
ERROR:
    glutPostRedisplay();
    _running = false;
    printf("error\n");
}

static void _mouse_reduce_hand(int button, int state, int x, int y)
{
    if (button != GLUT_LEFT_BUTTON || state != GLUT_DOWN || _running)
        return;
    if (_krk_done)
        return;
    _running = true;

    if (graph_krk(&_g, &_krk_edges))
        exit_err("graph_krk");
    _krk_done = true;

    glutPostRedisplay();
    _running = false;
}

static int _pixel_to_cell(struct pixel_ctx *ctx, GLint x, GLint y,
                          struct cell *cell)
{
    if (x < ctx->screen_x || x > (ctx->screen_w - ctx->screen_x))
        return -1;
    if (y < ctx->screen_y || y > (ctx->screen_h - ctx->screen_y))
        return -1;

    cell->row = ceil((y - ctx->screen_y) / ctx->cell_w);
    cell->col = ceil((x - ctx->screen_x) / ctx->cell_w);

    return 0;
}

static int _cell_to_gl(struct pixel_ctx *ctx, size_t row, size_t col,
                       double *x, double *y)
{
    double px_x;
    double px_y;

    px_x = ctx->screen_x + col * ctx->cell_w + ctx->cell_w / 2;
    px_y = ctx->screen_y + row * ctx->cell_w + ctx->cell_w / 2;
    px_x = px_x - ctx->screen_wh;
    px_y = ctx->screen_hh - px_y;

    *x = px_x / ctx->screen_wh;
    *y = px_y / ctx->screen_hh;

    return 0;
}

static int _rand_maze_nwall(struct point2d_map *map, struct graph *g,
                              size_t row_size, size_t col_size)
{
    size_t dsize;
    size_t row;
    size_t col;
    size_t cell_index;
    struct point2d *p;

    dsize = sizeof(*map->cells) * row_size * col_size;
    map->cells = malloc(dsize);
    if (map->cells == NULL)
        return -1;
    memset(map->cells, 0, dsize);
    map->row_size = row_size;
    map->col_size = col_size;

    if (graph_init(g, GRAPH_UDIRECTED))
        return -1;

    for (row = 0; row < row_size; row++) {
        for (col = 0; col < col_size; col++) {
            cell_index = row * col_size + col;
            p = map->cells + cell_index;
            p->type = POINT2D_WAY;
            p->in_path = false;
            p->height = 0;
            p->x = col;
            p->y = row;

            if (graph_add_vertex(g, cell_index, p->height, p))
                return -1;
        }
    }

    for (row = 0; row < row_size; row++) {
        for (col = 0; col < col_size; col++) {
            if (_map_link_vertex(map, g, row, col))
                return -1;
        }
    }

    return 0;
}

static void _render_reduce_hand(void)
{
    size_t row;
    size_t col;
    GLint viewport[4];
    GLint screen_w;
    GLint screen_h;
    GLint map_w;
    GLint map_h;
    GLint screen_x;
    GLint screen_y;
    struct rgb color;
    GLint x;
    GLint y;
    GLint cell_w;
    size_t row_size;
    size_t col_size;
    struct pset *e_set;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glGetIntegerv(GL_VIEWPORT, viewport);
    screen_w = viewport[2];
    screen_h = viewport[3];

    // find screen resolution
    row_size = _map.row_size + (_map.row_size - 1);
    col_size = _map.col_size + (_map.col_size - 1);
    map_w = col_size / row_size * screen_h;
    if (map_w <= screen_w) {
        map_h = screen_h;
        screen_x = (screen_w - map_w) / 2;
        screen_y = 0;
    } else {
        map_w = screen_w;
        map_h = screen_w * row_size / col_size;
        screen_x = 0;
        screen_y = (screen_h - map_h) / 2;
    }

    _ctx.screen_w = screen_w;
    _ctx.screen_h = screen_h;
    _ctx.screen_wh = screen_w / 2;
    _ctx.screen_hh = screen_h / 2;
    _ctx.screen_x = screen_x;
    _ctx.screen_y = screen_y;
    _ctx.cell_w = map_w / col_size;
    cell_w = _ctx.cell_w;

    // render cells
    color.red = 1.0f;
    color.green = 1.0f;
    color.blue = 1.0f;
    color.alpha = 1.0f;
    for (row = 0; row < row_size; row++) {
        for (col = 0; col < col_size; col++) {
            if (row % 2 != 0)
                break;
            if (col % 2 != 0)
                continue;

            // chose cordinate
            x = screen_x + col * cell_w;
            y = screen_y + row * cell_w;

            // render cell
            if (_render_cell(&_ctx, x, y, cell_w, color))
                exit_err("_render_cell");
        }
    }

    // render edges
    if (!_krk_done) {
        e_set = graph_edges(&_g);
        if (_render_edges(&_ctx, e_set, &COLOR_WHITE))
            exit_err("_render_edges");
    } else {
        if (_render_edges(&_ctx, &_krk_edges, &COLOR_WHITE))
            exit_err("_render_edges");
    }

    glutSwapBuffers();
}

static int _render_edges(struct pixel_ctx *ctx, struct pset *edges,
                         const struct rgb *color)
{
    struct graph_edge *e;
    struct point2d *p;
    double gl_x;
    double gl_y; 

    pset_rsiter(edges);
    glLineWidth(ctx->cell_w / 4);
    glBegin(GL_LINES);
    glColor3f(color->red, color->green, color->blue);
    for (;;) {
        e = pset_next(edges);
        if (e == NULL)
            break;
        
        // begin vertex
        p = e->begin->data;
        if (_cell_to_gl(&_ctx, 2 * p->y, 2 * p->x, &gl_x, &gl_y))
            return -1;
        glVertex2f(gl_x, gl_y);

        // end vertex
        p = e->end->data;
        if (_cell_to_gl(&_ctx, 2 * p->y, 2 * p->x, &gl_x, &gl_y))
            return -1;
        glVertex2f(gl_x, gl_y);
    }
    glEnd();

    return 0;
}

static void _cli_help(void)
{
    printf("USAGE\n");
    printf("    007 search size alg\n");
    printf("    007 reduce size\n");
    printf("    007 -h\n\n");

    printf("SIZE\n");
    printf("    tini         4x4 cells\n");
    printf("    small        16x16 cells\n");
    printf("    medium       32x32 cells\n");
    printf("    large        64x64 cells\n");

    printf("ALG\n");
    printf("    dfs         Depth First Search\n");
    printf("    bfd         Breath First Search\n");
    printf("    djt         Dijiktra\n");
}
