/* Sudoku generator/solver library.

Uses lower level functions declared in grid.h */

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <unistd.h>
#include "sudoku.h"

static Grid_T sudoku_generate_complete(void);
static void sudoku_init_choices(Grid_T *grid);
static int sudoku_try_next(Grid_T grid, int *row, int *col);
static void sudoku_set_rules(Grid_T *grid);
static int sudoku_errors_rules(Grid_T grid, int show, int num, int type);
static int sudoku_errors_zero(Grid_T grid, int show);
static void sudoku_add_choice(Grid_T *grid, int row, int col, int val);


/* sudoku_read: Reads a sudoku puzzle from stdin.

The input should have the format:
. 0 3 4 5 6 7 8 9 
4 5 6 7 8 9 1 2 3
7 8 9 1 2 3 4 5 6
2 3 4 5 6 7 8 9 1 
5 6 7 8 9 1 2 3 4
8 9 1 2 3 4 5 6 7 
3 4 5 6 7 8 9 1 2
6 7 8 9 1 2 3 4 5 
9 1 2 3 4 5 6 7 8 

Between each value there should be a space char. New lines are denoted by a
LF char right after the last digit. A '0' or '.' indicates an empty cell.

Parameters: void

Returns: a Grid_T type  */
Grid_T sudoku_read(void) {
    int i, j, val;
    Grid_T sudoku;

    for (i = 0; i < SIZE; i++) {
        if (i != 0) {
            val = getchar();

            /* check if we read a LF */
            if (val != 10) {
                grid_clear_formatok(&sudoku);
                return sudoku;
            }
        }
        for (j = 0; j < SIZE; j++) {
            val = getchar();

            /* read next char if we read a space */
            if (val == ' ') {
                val = getchar();
            }

            /* replace '.' with 0 */
            if (val == '.') {
                val = '0';
            }
            
            /* check if we are reading a digit */
            if (val < 48 || val > 57) {
                grid_clear_formatok(&sudoku);
                return sudoku;
            }
            grid_update_value(&sudoku, i, j, val - '0');
        }
    }
    grid_set_formatok(&sudoku);
    grid_reset_unique(&sudoku);
    grid_reset_rulesok(&sudoku);
    grid_clear_initialized(&sudoku);
    return sudoku;
}


/* sudoku_format_is_correct

Checks if the Grid_T returned by sudoku_read() has the correct format.

Parameters:
grid: a Grid_T type.

Returns: 1 if the format is OK, 0 otherwise */
int sudoku_format_is_correct(Grid_T grid) {
    return grid_read_formatok(grid);
}


/* sudoku_print: Writes a sudoku puzzle grid to stdout.

The format is 9 numbers per line and after each number there is a space char.
After the last number in each line there is a LF char.

Parameters:
grid: a Grid_T type.

Returns: void */
void sudoku_print(Grid_T grid) {
    int i, j, val;

    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
            val = grid_read_value(grid, i, j);
            if (j == SIZE - 1) {
                fprintf(stdout, "%d", val);
            }
            else {
                fprintf(stdout, "%d ", val);
            }
        }
        fprintf(stdout, "\n");
    }
    return;
}


/* sudoku_errors_rules

Writes to stdout the grid errors that appear in row or
column or block number indicated by num. Only errors related to numbers
appearing twice are considered.

Parameters:
grid: a Grid_T type.
show: 0 - error messages will not be printed and the function
       will return when the first error is found.
      1 - all error messages will be printed.
num: Indicates the number of row/col/block that will be checked for errors.
takes values from 0 to SIZE-1.
type: a value of 1 indicates a row, 0 indicates a column, 2 indicates a block.

Returns: 1 if errors were found, 0 otherwise. */
static int sudoku_errors_rules(Grid_T grid, int show, int num, int type) {
    int i, k, val, found[SIZE], count, err, row, col;

    err = 0;

    /* for each value from 1 to SIZE */
    for (k = 1; k <= SIZE; k++) {
        count = 0; 

        /* go through row or column or block */
        for (i = 0; i < SIZE; i++) {
            if (type == 1) {            /* row */
                val = grid_read_value(grid, num, i);
            }
            else if (type == 0) {       /* column */
                val = grid_read_value(grid, i, num);
            }
            else {                      /* block */
                row = i / BSIZE + (num / BSIZE) * BSIZE;
                col = i % BSIZE + (num % BSIZE) * BSIZE;
                val = grid_read_value(grid, row, col);
            }

            /* value was found count number of times */
            if (k == val) {     
                found[i] = 1;
                count++;
            }
            else {
                found[i] = 0;
            }
        }

        /* more than one time found => error */
        if (count > 1) {        
            err = 1;

            /* return if errors will not be printed */
            if (!show) {
                return err;
            }
            if (type == 1) {        /* error in row */
                fprintf(stdout, "In row %d, number %d appears in columns",
                    num + 1, k);
            }   
            else if (type == 0) {   /* error in column */
                fprintf(stdout, "In column %d, number %d appears in rows",
                    num + 1, k);
            }
            else {                  /* error in block */
                fprintf(stdout, "In block %d, number %d appears in cells",
                    num + 1, k);
            }

            /* print all cells that have errors */
            for (i = 0; i < SIZE; i++) {
                if (found[i]) {
                    fprintf(stdout, " %d", i + 1);
                }
            }
            fprintf(stdout, "\n");
        }
    }
    return err;
}


/* sudoku_errors_zero

Writes to stdout only the grid errors related to empty cells.

Parameters:
grid: a Grid_T type.
show: 0 - error messages will not be printed and the function
       will return when the first error is found.
      1 - all error messages will be printed.

Returns: 1 if errors were found, 0 otherwise. */
static int sudoku_errors_zero(Grid_T grid, int show) {
    int row, col, val, err, found, count_zero, count_nochoice;

    err = found = count_zero = count_nochoice = 0;
    for (row = 0; row < SIZE; row++) {
        for (col = 0; col < SIZE; col++) {
            val = grid_read_value(grid, row, col);

            /* only empty cells are considered */
            if (!val) {
                err = 1;

                /* return if errors will not be printed */
                if (!show) {
                    return err;
                }

                /* skip empty cells that have 0 choices */
                if (!grid_read_count(grid, row, col)) {
                    continue;
                }
                if (!found) {
                    fprintf(stdout, "Empty cells:");
                    found = 1;
                }
                count_zero++;
                fprintf(stdout, " (%d,%d)", row + 1, col + 1);
            }
        }
    }
    if (found && show) {
        fprintf(stdout, " [%d]\n", count_zero);
    }
    found = 0;
    for (row = 0; row < SIZE; row++) {
        for (col = 0; col < SIZE; col++) {
            val = grid_read_value(grid, row, col);

            /* only empty cells are considered */
            if (!val) {

                /* skip empty cells that have choices left */
                if (grid_read_count(grid, row, col)) {
                    continue;
                }
                if (!found) {
                    fprintf(stdout, "Empty cells with no choices:");
                    found = 1;
                }
                count_nochoice++;
                fprintf(stdout, " (%d,%d)", row + 1, col + 1);
            }
        }
    }
    if (found && show) {
        fprintf(stdout, " [%d]\n", count_nochoice);
    }
    return err;
}


/* sudoku_print_errors

Writes to stdout all errors of grid including:
    Errors related to numbers appearing twice in the same row/column/block.
    Errors related to empty cells.

Note that the number of available choices for each cell have been computed
when the function returns.

Parameters:
grid: a Grid_T type.
rules_only: Any value will print only the cells that violate rules, 0 will
also include empty cells.

Returns: void */
void sudoku_print_errors(Grid_T grid, int rules_only) {
    int k;

    /* Show errors related to numbers appearing twice in the same column */
    for (k = 0; k < SIZE; k++) {
        sudoku_errors_rules(grid, 1, k, 0);
    }

    /* Show errors related to numbers appearing twice in the same row */
    for (k = 0; k < SIZE; k++) {
        sudoku_errors_rules(grid, 1, k, 1);
    }

    /* Show errors related to numbers appearing twice in the same block */
    for (k = 0; k < SIZE; k++) {
        sudoku_errors_rules(grid, 1, k, 2);
    }

    /* Calculate available choices for each cell */
    if (!grid_is_initialized(grid)) {
        sudoku_init_choices(&grid);
    }

    /* Show errors related to empty cells */
    if (!rules_only) {
        sudoku_errors_zero(grid, 1);
    }
    return;
}


/* sudoku_is_correct

Checks if grid is fully completed and does not violate any sudoku rule.
Note that the number of available choices for each cell have been computed when
the function returns.

Parameters:
grid: a Grid_T type.
rules_only: Any value will check only for rules violation, 0 will
also include empty cells.

Returns:
rules_only != 0: 1 if puzzle is fully completed and does not violate rules,
                 0 otherwise.
rules_only = 0: 1 if puzzle violates only rules, 0 otherwise. */
int sudoku_is_correct(Grid_T grid, int rules_only) {

    /* Find errors related to numbers appearing twice in the same 
    row/column/clock */
    if (grid_read_rulesok(grid) == -1) {
        sudoku_set_rules(&grid);
    }
    if (!grid_read_rulesok(grid)) {
        return 0;
    }

    /* Calculate available choices for each cell */
    if (!grid_is_initialized(grid)) {
        sudoku_init_choices(&grid);
    }

    /* Find errors related to empty cells */
    if (!rules_only && sudoku_errors_zero(grid, 0)) {
        return 0;
    }
    return 1;
}


/* sudoku_init_choices

Calculates the available remaining choices for each
cell of the grid and modifies its appropriate fields as required.

Checks: if grid is NULL at runtime.

Parameters:
grid: a pointer to a Grid_T type.

Returns: void */
static void sudoku_init_choices(Grid_T *grid) {
    int row, col, k, val, bh, bv;

    assert(grid);
    for (row = 0; row < SIZE; row++) {
        for (col = 0; col < SIZE; col++) {
            val = grid_read_value(*grid, row, col);
            grid_clear_choice(grid, row, col, 0);

            /* for non-empty cells, set count to 0 and clear all choices */
            if (val) {
                grid_clear_count(grid, row, col);
                for (k = 1; k <= SIZE; k++) {
                    grid_clear_choice(grid, row, col, k);
                }
                continue;
            }

            /* for empty cells set count to SIZE and enable all choices-{0} */
            grid_set_count(grid, row, col);
            for (k = 1; k <= SIZE; k++) {
                grid_set_choice(grid, row, col, k);
            }

            /* remove rows, columns values from (row, col) */
            for (k = 0; k < SIZE; k++) {
                val = grid_read_value(*grid, row, k);
                grid_remove_choice(grid, row, col, val);
                val = grid_read_value(*grid, k, col);
                grid_remove_choice(grid, row, col, val);
            }

            /* remove block values from (row, col) */
            for (bh = SUBB(row); bh < SUBB(row) + BSIZE; bh++) {
                for (bv = SUBB(col); bv < SUBB(col) + BSIZE; bv++) {
                    val = grid_read_value(*grid, bh, bv);
                    grid_remove_choice(grid, row, col, val);
                }
            }
        }
    }
    return;
}


/* sudoku_set_rules

Modifies the rulesok field of the grid. It sets it to 1
if grid does not violate any sudoku rule and to 0 if it does violate a
rule. Only rules related to numbers appearing twice are considered.

Checks: if grid is NULL at runtime

Parameters:
grid: a pointer to a Grid_T type.

Returns: void */
static void sudoku_set_rules(Grid_T *grid) {
    int k;

    assert(grid);
    for (k = 0; k < SIZE; k++) {
        if (sudoku_errors_rules(*grid, 0, k, 0) ||        /* column conflict */
                sudoku_errors_rules(*grid, 0, k, 1) ||    /* row conflict */
                sudoku_errors_rules(*grid, 0, k, 2)) {    /* block conflict */
            grid_clear_rulesok(grid);
            return;
        }
    }
    grid_set_rulesok(grid);
    return;
}


/* sudoku_try_next

Finds a grid cell that has the minimum number of available choices and sets
the content of row, col pointers equal to the row, col of the cell.
If such cell does not exist, the content or row, col pointers is set to -1.

Checks: if row and col are NULL at runtime.

Parameters:
grid: a Grid_T type.
row: pointer to a row number.
col: pointer to a column number.

Returns: One of the available choices if there is a cell with a min number of
available choices, 0 otherwise */
static int sudoku_try_next(Grid_T grid, int *row, int *col) {
    int rowi, colj, val, count, count_min, num_scanned, non_empty;

    assert(row);
    assert(col);
    *row = -1;
    *col = -1;
    num_scanned = 0;
    non_empty = 0;
    count_min = CHOICES;

    /* Pick a random cell (rowi, colj) and scan grid horizontally starting
    from (rowi, colj) */
    rowi = rand() % SIZE;
    colj = rand() % SIZE;
    while (num_scanned != SIZE*SIZE) {
        while (colj != SIZE && num_scanned != SIZE*SIZE) {
            val = grid_read_value(grid, rowi, colj);

            /* skip non empty cells */
            if (val) {
                non_empty++;
                colj++;
                num_scanned++;
                continue;
            }

            /* puzzle is invalid if there is a cell with 0 choices */
            count = grid_read_count(grid, rowi, colj);
            if (count == 0) {
                return 0;
            }

            /* update min number of choices */
            if (count < count_min) {
                *row = rowi;
                *col = colj;
                count_min = count;
            }
            colj++;
            num_scanned++;
        }
        rowi++;
        if (rowi == SIZE) {
            rowi = 0;
        }
        colj = 0;
    }

    /* there is no cell with a minimum number of choices if all cells are
    non empty */
    if (non_empty == SIZE*SIZE) {
        return 0;
    }

    /* we have a cell selected, return one of its available choices */
    val = rand() % SIZE + 1;
    while (1) {
        if (grid_choice_is_valid(grid, *row, *col, val)) {
            break;
        }
        val++;
        if (val == SIZE + 1) {
            val = 1;
        }
    }
    return val;
}


/* sudoku_add_choice

Sets val as the value of grid cell (row, col) and removes val from the
available choices of every cell in the same row, column, block.

Checks: if grid is NULL at runtime.

Parameters:
grid: a pointer to a Grid_T type.
row: row number of val
col: column number of val

Returns: void */
static void sudoku_add_choice(Grid_T *grid, int row, int col, int val) {
    int k, bh, bv;
    
    assert(grid);
    grid_update_value(grid, row, col, val);

    /* set the number of choices to 0 and clear all choices */
    grid_clear_count(grid, row, col);
    for (k = 1; k <= SIZE; k++) {
        grid_clear_choice(grid, row, col, k);
    }

    /* remove the value as a choice of the same row/column */
    for (k = 0; k < SIZE; k++) {
        grid_remove_choice(grid, row, k, val);
        grid_remove_choice(grid, k, col, val);
    }

    /* remove the value as a choice of the same block */
    for (bh = SUBB(row); bh < SUBB(row) + BSIZE; bh++) {
        for (bv = SUBB(col); bv < SUBB(col) + BSIZE; bv++) {
            grid_remove_choice(grid, bh, bv, val);
        }
    }
    return;
}


/* sudoku_generate_complete

Generates a random valid sudoku with no empty cells.

Parameters: void

Returns: a Grid_T struct */
static Grid_T sudoku_generate_complete(void) {
    int k, row, col, val, tries;
    Grid_T sudoku;

    grid_set_unique(&sudoku);
    grid_set_rulesok(&sudoku);
    grid_clear_initialized(&sudoku);
    
    /* try to generate a full puzzle 10 times */
    for (tries = 0; tries < 10; tries++) {

        /* start with an empty puzzle and initialize cell choices */
        for (row = 0; row < SIZE; row++) {
            for (col = 0; col < SIZE; col++) {
                grid_update_value(&sudoku, row, col, 0);
                grid_set_count(&sudoku, row, col);
                grid_clear_choice(&sudoku, row, col, 0);
                for (k = 1; k <= SIZE; k++) {
                    grid_set_choice(&sudoku, row, col, k);
                }
            }
        }

        /* fill random cells starting from the ones that have the min number
        of choices */
        while((val = sudoku_try_next(sudoku, &row, &col))) {
            sudoku_add_choice(&sudoku, row, col, val);
        }

        /* puzzle is correct if it is fully completed */
        if (sudoku_is_correct(sudoku, 0)) {
            return sudoku;
        }
    }

    /* if failed to generate a full puzzle, return the one below.
    This puzzle however is trivial and shouldn't be returned */
    for (row = 0; row < SIZE; row++) {
        for (col = 0; col < SIZE; col++) {
            val = (col + row * BSIZE + row / BSIZE) % SIZE + 1;
            grid_update_value(&sudoku, row, col, val);
        }
    }
    return sudoku;
}


/* sudoku_generate

Generates a random sudoku with nelts non-zero cells.

The following algorithm is used:
1) Generate a random fully completed puzzle.
2) Start clearing random cells and each time check whether the
puzzle has a unique choice solution.
3) If the puzzle does not have unique choice solution, try another cell.
4) If no matter what cell is chosen, the puzzle cannot have a unique
choice solution, continue clearing cells until the nelts non-zero
cells requirement is met.

We can find whether the returned puzzle has a unique choice solution by
calling sudoku_solution_is_unique() before solving the puzzle.

Parameters:
nelts: number of non-zero cells.

Returns: a Grid_T type */
Grid_T sudoku_generate(int nelts) {
    int count, row, col, val , found, unique;
    Grid_T sudoku, gtmp, solved;

    /* tried(i,j) = 1 iff deleting cell (i, j) did not lead to a unique
    choice solvable puzzle  */
    int tried[SIZE][SIZE];

    assert(nelts >= 0 && nelts <= SIZE * SIZE);
    count = SIZE * SIZE - nelts;
    found = 0;
    unique = 1;

    for (row = 0; row < SIZE; row++) {
        for (col = 0; col < SIZE; col++) {
            tried[row][col] = 0;
        }
    }

    /* generate a random fully completed puzzle */
    sudoku = sudoku_generate_complete();

    while (count > 0) {

        /* pick a random non-empty cell (row, col) and attemp to clear it */
        row = rand() % SIZE;
        col = rand() % SIZE;
        if (!grid_read_value(sudoku, row, col)) {
            continue;
        }

        /* if puzzle still has a unique choice solution */
        if (unique) {

            /* pick a different cell if this one has been tried */
            if (tried[row][col]) {   
                continue;
            }

            /* initialize a copy of the puzzle and set the cell to 0 */
            gtmp = sudoku;
            grid_update_value(&gtmp, row, col, 0);
            sudoku_init_choices(&gtmp);

            /* solve the puzzle and if there is a unique choice solution,
            keep the copy of the puzzle, decrease count and set all cells
            to not tried */
            solved = sudoku_solve(gtmp);
            if (grid_read_unique(solved)) {
                count--;
                sudoku = gtmp;
                for (row = 0; row < SIZE; row++) {
                    for (col = 0; col < SIZE; col++) {
                        tried[row][col] = 0;
                    }
                }
                continue;
            }

            /* else if there was not a unique choice solution, set the cell to
            tried */
            tried[row][col] = 1;

            /* and search for a new non-tried cell */
            found = 0;
            for (row = 0; row < SIZE; row++) {
                for (col = 0; col < SIZE; col++) {
                    val = grid_read_value(sudoku, row, col);
                    if (val && !tried[row][col]) {
                        found = 1;
                    }
                }
            }

            /* if such cell cannot be found, setting any cell to 0 will
            produce a puzzle that will not have unique choice solution */ 
            if (!found) {
                unique = 0;
            }
        }

        /* if puzzle cannot have a unique choice solution, just set the
        cell to 0 and decrease count */
        else {
            count--;
            grid_update_value(&sudoku, row, col, 0);
        }
    }
    if (!unique) {
        grid_clear_unique(&sudoku);
    }
    return sudoku;
}


/* sudoku_solve

Solves grid using recursion (backtracking method). If there is more than
one solution, it returns one of them. If there is no solution, it returns one
of the attempted solutions. If the puzzle violates a sudoku rule, it returns
the initial puzzle.

Parameters:
grid: a Grid_T type

Returns: a Grid_T type */
Grid_T sudoku_solve(Grid_T grid) {
    int row, col, val;
    Grid_T gtmp;

    /* find if puzzle violates any rule */
    if (grid_read_rulesok(grid) == -1) {
        sudoku_set_rules(&grid);
    }

    /* return if grid violates sudoku rules */
    if (!grid_read_rulesok(grid)) {
        grid_clear_unique(&grid);
        return grid;
    }

    /* initialize choices once */
    if (!grid_is_initialized(grid)) {
        sudoku_init_choices(&grid);
        grid_set_initialized(&grid);
    }

    /* set unique for non auto generated puzzles */
    if (grid_read_unique(grid) == -1) {
        grid_set_unique(&grid);
    }

    /* find a cell with min number of choices  */
    while ((val = sudoku_try_next(grid, &row, &col))) {

        /* if there is a cell that has only 1 choice, fill it */
        if (grid_read_count(grid, row, col) == 1) {
            sudoku_add_choice(&grid, row, col, val);
        }

        /* else puzzle does not have a unique choice solution. solve a copy */
        else {
            grid_clear_unique(&grid);
            gtmp = grid;
            sudoku_add_choice(&gtmp, row, col, val);
            gtmp = sudoku_solve(gtmp);

            /* return the copy if solution is correct */
            if (sudoku_is_correct(gtmp, 0)) {
                return gtmp;
            }

            /* else remove the choice from the cell of the original puzzle */
            else {
                grid_remove_choice(&grid, row, col, val);
            }
        }
    }

    /* clear unique if puzzle not correct */
    if (grid_read_unique(grid) && !sudoku_is_correct(grid, 0)) {
        grid_clear_unique(&grid);
    }
    return grid;
}


/* sudoku_has_unique_choice_solution

Indicates whether grid has a unique choice solution.

Parameters:
grid: a Grid_T type

Returns:
1 if grid has a unique choice solution
0 if grid does not have a unique choice solution
-1 if it is not known whether grid has a unique choice solution */
int sudoku_has_unique_choice_solution(Grid_T grid) {
    return grid_read_unique(grid);
}
