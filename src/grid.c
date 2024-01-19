/* Grid interface: Lower level functions for manipulating a sudoku grid */

#include <stdio.h>
#include <assert.h>
#include "grid.h"


/* grid_update_value

Sets the value of grid cell (i, j) to n.

Checks: if grid is NULL
        if 0 <= i < SIZE
        if 0 <= j < SIZE
        if 0 <= n <= SIZE

Parameters:
grid: pointer to a Grid_T type
i: row index
j: column index
n:

Returns: void */
void grid_update_value(Grid_T *grid, int i, int j, int n) {
    assert(grid);
    assert(i >= 0 && i < SIZE);
    assert(j >= 0 && j < SIZE);
    assert(n >= 0 && n <= SIZE);
    grid->elts[i][j].val = n;
    return;
}


/* grid_read_value

Reads the value of grid cell (i, j).

Checks: if 0 <= i < SIZE
        if 0 <= j < SIZE

Parameters:
grid: a Grid_T type
i: row index
j: column index

Returns: a value from 0 to SIZE */
int grid_read_value(Grid_T grid, int i, int j) {
    assert(i >= 0 && i < SIZE);
    assert(j >= 0 && j < SIZE);
    return grid.elts[i][j].val;
}


/* grid_set_choice

Enables choice n for grid cell (i, j).

Checks: if grid is NULL
        if 0 <= i < SIZE
        if 0 <= j < SIZE
        if 0 <= n <= SIZE

Parameters:
grid: pointer to a Grid_T type
i: row index
j: column index
n:

Returns: void */
void grid_set_choice(Grid_T *grid, int i, int j, int n) {
    assert(grid);
    assert(i >= 0 && i < SIZE);
    assert(j >= 0 && j < SIZE);
    assert(n >= 0 && n <= SIZE);
    grid->elts[i][j].choices.num[n] = 1;
    return;
}


/* grid_clear_choice

Clears choice n for grid cell (i, j).

Checks: if grid is NULL
        if 0 <= i < SIZE
        if 0 <= j < SIZE
        if 0 <= n <= SIZE

Parameters:
grid: pointer to a Grid_T type
i: row index
j: column index
n:

Returns: void */
void grid_clear_choice(Grid_T *grid, int i, int j, int n) {
    assert(grid);
    assert(i >= 0 && i < SIZE);
    assert(j >= 0 && j < SIZE);
    assert(n >= 0 && n <= SIZE);
    grid->elts[i][j].choices.num[n] = 0;
    return;
}


/* grid_choice_is_valid

Finds if n is an available choice for grid cell (i, j).

Checks: if 0 <= i < SIZE
        if 0 <= j < SIZE
        if 0 <= n <= SIZE

Parameters:
grid: a Grid_T type
i: row index
j: column index
n:

Returns: 1 or 0 indicating whether a choice is valid or not */
int grid_choice_is_valid(Grid_T grid, int i, int j, int n) {
    assert(i >= 0 && i < SIZE);
    assert(j >= 0 && j < SIZE);
    assert(n >= 0 && n <= SIZE);
    return grid.elts[i][j].choices.num[n];
}


/* grid_remove_choice

Removes choice n from grid cell (i, j) and adjusts its number of
available remaining choices.

Checks: if grid is NULL
        if 0 <= i < SIZE
        if 0 <= j < SIZE
        if 0 <= n <= SIZE

Parameters:
grid: pointer to a Grid_T type
i: row index
j: column index
n:

Returns: void */
void grid_remove_choice(Grid_T *grid, int i, int j, int n) {
    assert(grid);
    assert(i >= 0 && i < SIZE);
    assert(j >= 0 && j < SIZE);
    assert(n >= 0 && n <= SIZE);
    if (grid->elts[i][j].choices.num[n]) {
        grid->elts[i][j].choices.num[n] = 0;
        grid->elts[i][j].choices.count--;
    }
    return;
}


/* grid_read_count

Reads the available choices for grid cell(i, j).

Checks: if 0 <= i < SIZE
        if 0 <= j < SIZE

Parameters:
grid: a Grid_T type
i: row index
j: column index

Returns: the number of available choices for grid cell (i, j) */
int grid_read_count(Grid_T grid, int i, int j) {
    assert(i >= 0 && i < SIZE);
    assert(j >= 0 && j < SIZE);
    return grid.elts[i][j].choices.count;
}


/* grid_set_count

Sets the number of available choices for grid cell (i, j) to SIZE.

Checks: if grid is NULL
        if 0 <= i < SIZE
        if 0 <= j < SIZE

Parameters:
grid: pointer to a Grid_T type
i: row index
j: column index

Returns: void */
void grid_set_count(Grid_T *grid, int i, int j) {
    assert(grid);
    assert(i >= 0 && i < SIZE);
    assert(j >= 0 && j < SIZE);
    grid->elts[i][j].choices.count = SIZE;
    return;
}


/* grid_clear_count

Sets the number of available choices for grid cell (i, j) to 0.

Checks: if grid is NULL
        if 0 <= i < SIZE
        if 0 <= j < SIZE

Parameters:
grid: pointer to a Grid_T type
i: row index
j: column index

Returns: void */
void grid_clear_count(Grid_T *grid, int i, int j) {
    assert(grid);
    assert(i >= 0 && i < SIZE);
    assert(j >= 0 && j < SIZE);
    grid->elts[i][j].choices.count = 0;
    return;
}


/* grid_read_unique

Reads the unique field of grid.

Parameters:
grid: a Grid_T type

Returns:
- 1 if grid has a unique choice solution.
- 0 if it grid does not have a unique choice solution.
- else -1. */
int grid_read_unique(Grid_T grid) {
    return grid.unique;
}


/* grid_set_unique

Sets the unique field of grid to 1, meaning it has a unique choice solution.

Checks: if grid is NULL

Parameters:
grid: pointer to a Grid_T type

Returns: void */
void grid_set_unique(Grid_T *grid) {
    assert(grid);
    grid->unique = 1;
    return;
}


/* grid_clear_unique

Sets the unique field of grid to 0, meaning it does not have a unique choice
solution.

Checks: if grid is NULL

Parameters:
grid: pointer to a Grid_T type

Returns: void */
void grid_clear_unique(Grid_T *grid) {
    assert(grid);
    grid->unique = 0;
    return;
}


/* grid_reset_unique

Sets the unique field of grid to -1, meaning it is not known if it has a unique
choice solution.

Checks: if grid is NULL

Parameters:
grid: pointer to a Grid_T type

Returns: void */
void grid_reset_unique(Grid_T *grid) {
    assert(grid);
    grid->unique = -1;
    return;
}


/* grid_set_rulesok

Sets the rulesok field of grid to 1, meaning it does not violate any
sudoku rule.

Checks: if grid is NULL

Parameters:
grid: pointer to a Grid_T type

Returns: void */
void grid_set_rulesok(Grid_T *grid) {
    assert(grid);
    grid->rulesok = 1;
}


/* grid_clear_rulesok

Sets the rulesok field of grid to 0, meaning it violates at least one
sudoku rule.

Checks: if grid is NULL

Parameters:
grid: pointer to a Grid_T type

Returns: void */
void grid_clear_rulesok(Grid_T *grid) {
    assert(grid);
    grid->rulesok = 0;
}


/* grid_reset_rulesok

Sets the rulesok field of grid to -1, meaning it is not known if it violates
any rules.

Checks: if grid is NULL

Parameters:
grid: pointer to a Grid_T type

Returns: void */
void grid_reset_rulesok(Grid_T *grid) {
    assert(grid);
    grid->rulesok = -1;
}


/* grid_read_rulesok

Reads the rulesok field of grid.

Parameters:
grid: a Grid_T type

Returns: 1 if grid does not violate any sudoku rule. 0 if grid violates
at least one rule, -1 otherwise */
int grid_read_rulesok(Grid_T grid) {
    return grid.rulesok;
}


/* grid_set_initialized

Sets the initialized field of grid to 1, meaning the available choices for each
cell have been computed.

Checks: if grid is NULL

Parameters:
grid: pointer to a Grid_T type

Returns: void */
void grid_set_initialized(Grid_T *grid) {
    assert(grid);
    grid->initialized = 1;
}


/* grid_clear_initialized

Sets the initialized field of grid to 0, meaning the available choices for each
cell have not been computed.

Checks: if grid is NULL

Parameters:
grid: pointer to a Grid_T type

Returns: void */
void grid_clear_initialized(Grid_T *grid) {
    assert(grid);
    grid->initialized = 0;
}


/* grid_is_initialized

Reads the initialized field of grid.

Parameters:
grid: a Grid_T type

Returns: 1 if the available choices for each cell have been computed,
0 otherwise */
int grid_is_initialized(Grid_T grid) {
    return grid.initialized;
}


/* grid_set_formatok

Sets the formatok field of grid to 1, meaning all cells have valid values.

Checks: if grid is NULL

Returns: void */
void grid_set_formatok(Grid_T *grid) {
    assert(grid);
    grid->formatok = 1;
}


/* grid_clear_formatok

Sets the formatok field of grid to 0, meaning the grid does not match the
required input format.

Checks: if grid is NULL

Returns: void */
void grid_clear_formatok(Grid_T *grid) {
    assert(grid);
    grid->formatok = 0;
}


/* grid_read_formatok

Reads the formatok field of grid.

Parameters:
grid: a Grid_T type

Returns: 1 if the format is OK, 0 otherwise */
int grid_read_formatok(Grid_T grid) {
    return grid.formatok;
}
