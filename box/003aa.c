/*
SYNOPSIS
========

    Assignment 003aa
    Display Sierpinski.
    ADT: None
    Algorithm: Recursion

USAGE
=====

    $ ./dest/bin/003aa -n 6
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <GL/glut.h>

#include <cs106b/error.h>

#define SIER_NSTEP_MIN 1
#define SIER_NSTEP_MAX 16
#define SIER_NSTEP_DEF 6

size_t sier_nstep;

void render_screen(void);
void sierpinski(float ax, float ay, float bx, float by, float cx, float cy, 
                size_t n);
void cli_help(void);

int main(int argc, char *argv[])
{
    int opt;

    sier_nstep = SIER_NSTEP_DEF;
    for (;;) {
        opt = getopt(argc, argv, "hn:");
        if (opt < 0)
            break;

        if (opt == 'h') {
            cli_help();
            return EXIT_SUCCESS;
        } else if (opt == 'n') {
            sier_nstep = atoi(optarg);
            if (sier_nstep < SIER_NSTEP_MIN || sier_nstep > SIER_NSTEP_MAX) {
                cli_help();
                return EXIT_FAILURE;
            }
        } else {
            cli_help();
            return EXIT_FAILURE;
        }
    }

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(800, 600);
    glutCreateWindow("cs106b.008.fractals");

    glutDisplayFunc(render_screen);
    glutMainLoop();

    return EXIT_SUCCESS;
}

void render_screen(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLineWidth(1);
    glColor3f(1.0, 1.0, 1.0);

    sierpinski(0, 1, 1, -1, -1, -1, sier_nstep);

    glutSwapBuffers();
}

void sierpinski(float ax, float ay, float bx, float by, float cx, float cy,
                size_t n)
{
    float ac_mx, ac_my;
    float ab_mx, ab_my;
    float bc_mx, bc_my;

    if (n == 0)
        return;

    glBegin(GL_LINE_LOOP);
    glVertex2f(ax, ay);
    glVertex2f(bx, by);
    glVertex2f(cx, cy);
    glEnd();

    ac_mx = (ax + cx) / 2;
    ac_my = (ay + cy) / 2;
    ab_mx = (ax + bx) / 2;
    ab_my = (ay + by) / 2;
    bc_mx = (bx + cx) / 2;
    bc_my = (by + cy) / 2;

    sierpinski(ax, ay, ac_mx, ac_my, ab_mx, ab_my, n - 1);
    sierpinski(cx, cy, ac_mx, ac_my, bc_mx, bc_my, n - 1);
    sierpinski(bx, by, ab_mx, ab_my, bc_mx, bc_my, n - 1);
}

void cli_help(void)
{
    printf("USAGE\n");
    printf("    003aa [-n steps]\n");
    printf("    003aa -h\n\n");
    
    printf("OPTIONS\n");
    printf("    -n      Number of steps, default is %u\n", SIER_NSTEP_DEF);
    printf("            %u <= n <= %u\n", SIER_NSTEP_MIN, SIER_NSTEP_MAX);
}
