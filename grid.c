/* Grid interface: Lower level functions for manipulating a sudoku grid.

Declarations are in grid.h */

#include <stdio.h>
#include <assert.h>
#include "grid.h"


/* Description: See grid.h */
void grid_update_value(Grid_T *grid, int i, int j, int n) {
    assert(grid);
    assert(n >=0 && n <= SIZE);
    grid->elts[i][j].val = n;
    return;
}

/* Description: See grid.h */
int grid_read_value(Grid_T grid, int i, int j) {
    return grid.elts[i][j].val;
}

/* Description: See grid.h */
void grid_set_choice(Grid_T *grid, int i, int j, int n) {
    assert(grid);
    assert(n >= 0 && n <= SIZE);
    grid->elts[i][j].choices.num[n] = 1;
    return;
}

/* Description: See grid.h */
void grid_clear_choice(Grid_T *grid, int i, int j, int n) {
    assert(grid);
    assert(n >= 0 && n <= SIZE);
    grid->elts[i][j].choices.num[n] = 0;
    return;
}

/* Description: See grid.h */
int grid_choice_is_valid(Grid_T grid, int i, int j, int n) {
    assert(n >= 0 && n <= SIZE);
    return grid.elts[i][j].choices.num[n];
}

/* Description: See grid.h */
void grid_remove_choice(Grid_T *grid, int i, int j, int n) {
    assert(grid);
    assert(n >= 0 && n <= 9);
    if (grid->elts[i][j].choices.num[n]) {
        grid->elts[i][j].choices.num[n] = 0;
        grid->elts[i][j].choices.count--;
    }
    return;
}

/* Description: See grid.h */
int grid_read_count(Grid_T grid, int i, int j) {
    return grid.elts[i][j].choices.count;
}

/* Description: See grid.h */
void grid_set_count(Grid_T *grid, int i, int j) {
    assert(grid);
    grid->elts[i][j].choices.count = SIZE;
    return;
}

/* Description: See grid.h */
void grid_clear_count(Grid_T *grid, int i, int j) {
    assert(grid);
    grid->elts[i][j].choices.count = 0;
    return;
}

/* Description: See grid.h */
int grid_read_unique(Grid_T grid) {
    return grid.unique;
}

/* Description: See grid.h */
void grid_set_unique(Grid_T *grid) {
    assert(grid);
    grid->unique = 1;
    return;
}

/* Description: See grid.h */
void grid_clear_unique(Grid_T *grid) {
    assert(grid);
    grid->unique = 0;
    return;
}

/* Description: See grid.h */
void grid_reset_unique(Grid_T *grid) {
    assert(grid);
    grid->unique = -1;
    return;
}

/* Description: See grid.h */
void grid_set_rulesok(Grid_T *grid) {
    grid->rulesok = 1;
}

/* Description: See grid.h */
void grid_clear_rulesok(Grid_T *grid) {
    grid->rulesok = 0;
}

/* Description: See grid.h */
void grid_reset_rulesok(Grid_T *grid) {
    grid->rulesok = -1;
}

/* Description: See grid.h */
int grid_read_rulesok(Grid_T grid) {
    return grid.rulesok;
}

/* Description: See grid.h */
void grid_set_initialized(Grid_T *grid) {
    grid->initialized = 1;
}

/* Description: See grid.h */
void grid_clear_initialized(Grid_T *grid) {
    grid->initialized = 0;
}

/* Description: See grid.h */
int grid_is_initialized(Grid_T grid) {
    return grid.initialized;
}