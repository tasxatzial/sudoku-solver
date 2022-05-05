/* Grid interface: Lower level functions for manipulating a sudoku grid */

#ifndef _GRID_H_
#define _GRID_H_

#include <stdio.h>

#define SIZE 9      /* grid size 9x9 */
#define BSIZE 3     /* grid block size 3x3 */
#define CHOICES 10  /* maximum number of available choices for each cell */

/* calculates the maximum multiple of BSIZE that is less than i */
#define SUBB(i) (((i)/BSIZE) * BSIZE)


/* Struct that represents a sudoku puzzle.

Fields:
> unique:
-1: unknown whether puzzle has a unique choice solution.
1: puzzle has a unique choice solution.
0: puzzle does not have a unique choice solution.

> rulesok:
0: puzzle violates a sudoku rule.
1: puzzle does not violate any sudoku rule.
-1: unknown whether puzzle violates any sudoku rules. Only rules related to
    numbers appearing twice are considered.

> initialized: 1 if the available choices for each cell have been computed
and are up to date, 0 otherwise.
> val: value of cell i,j. Takes values from 0 to SIZE. A 0 value means that
    the cell is currently empty.
> count: number of possible choices for cell i,j
> num[CHOICES]: num[k] = 1 if k is valid choice for cell(i, j) else num[k] = 0
> elts[SIZE][SIZE]: the actual grid
*/
typedef struct grid_s {
    int unique;
    int rulesok;
    int initialized;
    int formatok;
    struct {
        int val;
        struct {
            int count;
            int num[CHOICES];
        } choices;
    } elts[SIZE][SIZE];
} Grid_T;


/* grid_update_value

Sets the value of grid cell (i, j) to n.

Checks: if grid is NULL at runtime
        if n is from 0 to SIZE

Parameters:
grid: pointer to a Grid_T type
i: row number from 0 to SIZE-1
j: column number from 0 to SIZE-1
n: value from 0 to SIZE

Returns: void */
void grid_update_value(Grid_T *grid, int i, int j, int n);


/* grid_read_value

Reads the value of grid cell (i, j).

Parameters:
grid: a Grid_T type
i: row number from 0 to SIZE-1
j: column number from 0 to SIZE-1

Returns: a value from 0 to SIZE */
int grid_read_value(Grid_T grid, int i, int j);


/* grid_set_choice

Enables choice n for grid cell (i, j).

Checks: if grid is NULL at runtime
        if n is from 0 to SIZE

Parameters:
grid: pointer to a Grid_T type
i: row number from 0 to SIZE-1
j: column number from 0 to SIZE-1
n: value from 0 to SIZE

Returns: void */
void grid_set_choice(Grid_T *grid, int i, int j, int n);


/* grid_clear_choice

Clears choice n for grid cell (i, j).

Checks: if grid is NULL at runtime
        if n is from 0 to SIZE

Parameters:
grid: pointer to a Grid_T type
i: row number from 0 to SIZE-1
j: column number from 0 to SIZE-1
n: value from 0 to SIZE

Returns: void */
void grid_clear_choice(Grid_T *grid, int i, int j, int n);


/* grid_choice_is_valid

Finds if n is an available choice for grid cell (i, j).

Checks: if n is from 0 to SIZE

Parameters:
grid: a Grid_T type
i: row number from 0 to SIZE-1
j: column number from 0 to SIZE-1
n: value from 0 to SIZE

Returns: 1 or 0 indicating whether a choice is valid or not */
int grid_choice_is_valid(Grid_T grid, int i, int j, int n);


/* grid_remove_choice

Removes choice n from grid cell (i, j) and adjusts its number of
available remaining choices.

Checks: if grid is NULL at runtime
        if n is from 0 to SIZE

Parameters:
grid: pointer to a Grid_T type
i: row number from 0 to SIZE-1
j: column number from 0 to SIZE-1
n: value from 0 to SIZE

Returns: void */
void grid_remove_choice(Grid_T *grid, int i, int j, int n);


/* grid_read_count

Reads the available choices for grid cell(i, j).

Parameters:
grid: a Grid_T type
i: row number from 0 to SIZE-1
j: column number from 0 to SIZE-1

Returns: the number of available choices for grid cell (i, j) */
int grid_read_count(Grid_T grid, int i, int j);


/* grid_set_count

Sets the number of available choices for grid cell (i, j) to SIZE.

Checks: if grid is NULL at runtime

Parameters:
grid: pointer to a Grid_T type
i: row number from 0 to SIZE-1
j: column number from 0 to SIZE-1

Returns: void */
void grid_set_count(Grid_T *grid, int i, int j);


/* grid_clear_count

Sets the number of available choices for grid cell (i, j) to 0.

Checks: if grid is NULL at runtime

Parameters:
grid: pointer to a Grid_T type
i: row number from 0 to SIZE-1
j: column number from 0 to SIZE-1

Returns: void */
void grid_clear_count(Grid_T *grid, int i, int j);


/* grid_read_unique

Reads the unique field of grid.

Parameters:
grid: a Grid_T type

Returns: 1 if grid has a unique choice solution, 0 when not, -1 otherwise */
int grid_read_unique(Grid_T grid);


/* grid_set_unique

Sets the unique field of grid to 1, meaning it has a unique choice solution.

Checks: if grid is NULL at runtime

Parameters:
grid: pointer to a Grid_T type

Returns: void */
void grid_set_unique(Grid_T *grid);


/* grid_clear_unique

Sets the unique field of grid to 0, meaning it does not have a unique choice
solution.

Checks: if grid is NULL at runtime

Parameters:
grid: pointer to a Grid_T type

Returns: void */
void grid_clear_unique(Grid_T *grid);


/* grid_reset_unique

Sets the unique field of grid to -1, meaning it is not known if it has a unique
choice solution.

Checks: if grid is NULL at runtime

Parameters:
grid: pointer to a Grid_T type

Returns: void */
void grid_reset_unique(Grid_T *grid);


/* grid_set_rulesok

Sets the rulesok field of grid to 1, meaning it does not violate any
sudoku rule. Only rules related to numbers appearing twice should be considered.

Checks: if grid is NULL at runtime

Parameters:
grid: pointer to a Grid_T type

Returns: void */
void grid_set_rulesok(Grid_T *grid);


/* grid_clear_rulesok

Sets the rulesok field of grid to 0, meaning it violates at least one
sudoku rule. Only rules related to numbers appearing twice should be considered.

Checks: if grid is NULL at runtime

Parameters:
grid: pointer to a Grid_T type

Returns: void */
void grid_clear_rulesok(Grid_T *grid);


/* grid_reset_rulesok

Sets the rulesok field of grid to -1, meaning it is not known if it violates
any rules. Only rules related to numbers appearing twice should be considered.

Checks: if grid is NULL at runtime

Parameters:
grid: pointer to a Grid_T type

Returns: void */
void grid_reset_rulesok(Grid_T *grid);


/* grid_read_rulesok

Reads the rulesok field of grid.

Parameters:
grid: a Grid_T type

Returns: 1 if grid does not violate any sudoku rule. 0 if grid violates
at least one rule, -1 otherwise */
int grid_read_rulesok(Grid_T grid);


/* grid_set_initialized

Sets the initialized field of grid to 1, meaning the available choices for each
cell have been computed and are up to date.

Checks: if grid is NULL at runtime

Parameters:
grid: pointer to a Grid_T type

Returns: void */
void grid_set_initialized(Grid_T *grid);


/* grid_clear_initialized

Sets the initialized field of grid to 0, meaning the available choices for each
cell have not been computed or are not up to date.

Checks: if grid is NULL at runtime

Parameters:
grid: pointer to a Grid_T type

Returns: void */
void grid_clear_initialized(Grid_T *grid);


/* grid_is_initialized

Reads the initialized field of grid.

Parameters:
grid: a Grid_T type

Returns: 1 if the available choices for each cell have been computed and are
up to date, 0 otherwise */
int grid_is_initialized(Grid_T grid);


/* grid_set_formatok

Sets the formatok field of grid to 1, meaning all grid cells have valid values.

Checks: if grid is NULL at runtime

Returns: void */
void grid_set_formatok(Grid_T *grid);


/* grid_clear_formatok

Sets the formatok field of grid to 0, meaning the grid has at least one invalid 
cell value.

Checks: if grid is NULL at runtime

Returns: void */
void grid_clear_formatok(Grid_T *grid);


/* grid_read_formatok

Reads the formatok field of grid.

Parameters:
grid: a Grid_T type

Returns: 1 if the format is OK, 0 otherwise */
int grid_read_formatok(Grid_T grid);

#endif
