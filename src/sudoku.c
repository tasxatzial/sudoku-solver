/* Sudoku generator/solver library.

Uses lower level functions declared in grid.h */

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <unistd.h>
#include "sudoku.h"

static Grid_T sudoku_generate_complete(void);
static void sudoku_init_choices(Grid_T *grid);
static void sudoku_set_choice(Grid_T *grid, int row, int col, int val);
static int sudoku_try_next(Grid_T grid, int *row, int *col);
static void sudoku_set_rules(Grid_T *grid);
static int sudoku_errors_rules(Grid_T grid, int show, int index, int type);
static int sudoku_errors_empty(Grid_T grid, int show);


/* sudoku_read: Reads a sudoku from stdin.

The accepted format is 9 numbers per line. After each number there is a space
char. Empty cells are denoted by a dot char. After the last cell in each line
there is a LF char.

Example:
1 . . . . 7 . 9 .
. 3 . . 2 . . . 8
. . 9 6 . . 5 . .
. . 5 3 . . 9 . .
. 1 . . 8 . . . 2
6 . . . . 4 . . .
3 . . . . . . 1 .
. 4 . . . . . . 7
. . 7 . . . 3 . .

Parameters: void

Returns: a Grid_T type. */
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

Indicates whether the given sudoku has the correct format.

Parameters:
grid: a Grid_T type.

Returns: 1 if the format is OK, 0 otherwise */
int sudoku_format_is_correct(Grid_T grid) {
    return grid_read_formatok(grid);
}


/* sudoku_print: Writes a sudoku puzzle grid to stdout.

The format is 9 numbers per line. After each number there is a space char.
Empty cells are denoted by a dot char. After the last cell in each line there
is a LF char.

Parameters:
grid: a Grid_T type.

Returns: void */
void sudoku_print(Grid_T grid) {
    int i, j, val;

    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
            val = grid_read_value(grid, i, j);
            if (val) {
                fprintf(stdout, "%d", val);
            } else {
                fprintf(stdout, ".");
            }
            if (j != SIZE - 1) {
                fprintf(stdout, " ");
            }
        }
        fprintf(stdout, "\n");
    }
    return;
}


/* sudoku_errors_rules

Writes to stdout the sudoku errors that appear in the row or
column or block indicated by index. Only errors related to numbers
appearing twice are considered.

Parameters:
grid: a Grid_T type.
show: 0 - error messages will not be printed and the function
      will return when the first error is found.
      1 - all error messages will be printed.
index: Indicates the index of row or column or block that will be checked
       for errors.
type: a value of 1 indicates a row, 0 indicates a column, 2 indicates a block.

Returns: 1 if errors were found, 0 otherwise. */
static int sudoku_errors_rules(Grid_T grid, int show, int index, int type) {
    int i, val, val_cell, found[SIZE], count, err, row, col;

    err = 0;
    for (val = 1; val <= SIZE; val++) {
        count = 0; 

        /* scan the row/column/block i (each have SIZE cells) */
        for (i = 0; i < SIZE; i++) {
            if (type == 1) {            /* row */
                val_cell = grid_read_value(grid, index, i);
            }
            else if (type == 0) {       /* column */
                val_cell = grid_read_value(grid, i, index);
            }
            else {                      /* block */
                row = i / BSIZE + (index / BSIZE) * BSIZE;
                col = i % BSIZE + (index % BSIZE) * BSIZE;
                val_cell = grid_read_value(grid, row, col);
            }

            /* cell value was found in the same row/column/block */
            if (val == val_cell) {
                found[i] = 1;
                count++;
            }
            else {
                found[i] = 0;
            }
        }

        /* more than one time found in the same row/column/block => error */
        if (count > 1) {        
            err = 1;

            /* return if we don't want errors to be printed */
            if (!show) {
                return err;
            }

            if (type == 1) {        /* error in row */
                fprintf(stdout, "In row %d, number %d appears in columns", index + 1, val);
            }   
            else if (type == 0) {   /* error in column */
                fprintf(stdout, "In column %d, number %d appears in rows", index + 1, val);
            }
            else {                  /* error in block */
                fprintf(stdout, "In block %d, number %d appears in cells", index + 1, val);
            }

            /* print all cells in the same row/col/block that have errors */
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


/* sudoku_errors_empty

Writes to stdout the grid errors related to empty cells.

Parameters:
grid: a Grid_T type.
show: 0 - error messages will not be printed and the function
       will return when the first error is found.
      1 - all error messages will be printed.

Returns: 1 if errors were found, 0 otherwise. */
static int sudoku_errors_empty(Grid_T grid, int show) {
    int row, col, val, err, first_print;

    err = first_print = 0;
    for (row = 0; row < SIZE; row++) {
        for (col = 0; col < SIZE; col++) {
            val = grid_read_value(grid, row, col);

            /* it's an error if cell is empty */
            if (!val) {
                err = 1;

                /* return if we don't want errors to be printed */
                if (!show) {
                    return err;
                }

                /* print only the first time an error is found */
                if (!first_print) {
                    fprintf(stdout, "Empty cells:");
                    first_print = 1;
                }
                fprintf(stdout, " (%d,%d)", row + 1, col + 1);
            }
        }
    }
    if (first_print && show) {
        fprintf(stdout, " \n");
    }
    return err;
}


/* sudoku_print_errors

Writes to stdout all sudoku errors including:
    Errors related to numbers appearing twice in the same row/column/block.
    Errors related to empty cells.

Parameters:
grid: a Grid_T type.
rules_only: Any non-zero value will print only the cells that violate rules,
0 will also show all empty cells.

Returns: void */
void sudoku_print_errors(Grid_T grid, int rules_only) {
    int i;

    /* Show errors related to numbers appearing twice in the same column */
    for (i = 0; i < SIZE; i++) {
        sudoku_errors_rules(grid, 1, i, 0);
    }

    /* Show errors related to numbers appearing twice in the same row */
    for (i = 0; i < SIZE; i++) {
        sudoku_errors_rules(grid, 1, i, 1);
    }

    /* Show errors related to numbers appearing twice in the same block */
    for (i = 0; i < SIZE; i++) {
        sudoku_errors_rules(grid, 1, i, 2);
    }

    /* Show errors related to empty cells */
    if (!rules_only) {
        sudoku_errors_empty(grid, 1);
    }
    return;
}


/* sudoku_is_correct

Checks if the given sudoku is fully completed and does not violate any rule.

Parameters:
grid: a Grid_T type.
rules_only: Any value will check only for rules violation, 0 will
also show all empty cells.

Returns:
rules_only != 0: 1 if puzzle does not violate rules, else 0.
rules_only = 0: 1 if puzzle is completed and does not violate rules, else 0. */
int sudoku_is_correct(Grid_T grid, int rules_only) {

    /* Errors related to numbers appearing twice in the same 
    row/column/clock */
    sudoku_set_rules(&grid);
    if (!grid_read_rulesok(grid)) {
        return 0;
    }

    /* Errors related to empty cells */
    if (!rules_only && sudoku_errors_empty(grid, 0)) {
        return 0;
    }
    return 1;
}


/* sudoku_init_choices

Calculates the available remaining choices for each cell of the given sudoku
and modifies its appropriate fields as required.

Checks: if grid is NULL.

Parameters:
grid: a pointer to a Grid_T type.

Returns: void */
static void sudoku_init_choices(Grid_T *grid) {
    int i, val, choice, row, col, brow, bcol;

    assert(grid);
    for (row = 0; row < SIZE; row++) {
        for (col = 0; col < SIZE; col++) {
            val = grid_read_value(*grid, row, col);
            grid_clear_choice(grid, row, col, 0);

            /* for filled-in cells set count to 0 and clear all choices */
            if (val) {
                grid_clear_count(grid, row, col);
                for (choice = 1; choice <= SIZE; choice++) {
                    grid_clear_choice(grid, row, col, choice);
                }
                continue;
            }

            /* else set count to SIZE and enable all choices-{0} */
            grid_set_count(grid, row, col);
            for (choice = 1; choice <= SIZE; choice++) {
                grid_set_choice(grid, row, col, choice);
            }

            /* remove the value of (row, col) from the choices of all
            cells that belong to the row & col that contains (row, col) */
            for (i = 0; i < SIZE; i++) {
                val = grid_read_value(*grid, row, i);
                grid_remove_choice(grid, row, col, val);
                val = grid_read_value(*grid, i, col);
                grid_remove_choice(grid, row, col, val);
            }

            /* remove the value of (row, col) from the choices of all cells
            that belong to the block that contains (row, col) */
            for (brow = SUBB(row); brow < SUBB(row) + BSIZE; brow++) {
                for (bcol = SUBB(col); bcol < SUBB(col) + BSIZE; bcol++) {
                    val = grid_read_value(*grid, brow, bcol);
                    grid_remove_choice(grid, row, col, val);
                }
            }
        }
    }
    return;
}


/* sudoku_set_rules

Modifies the rulesok field of the given sudoku. It sets it to 1
if grid does not violate any sudoku rule and to 0 if it does violate a
rule. Only rules related to numbers appearing twice are considered.

Checks: if grid is NULL.

Parameters:
grid: a pointer to a Grid_T type.

Returns: void */
static void sudoku_set_rules(Grid_T *grid) {
    int i;

    assert(grid);
    for (i = 0; i < SIZE; i++) {
        if (sudoku_errors_rules(*grid, 0, i, 0) ||    /* column conflict */
            sudoku_errors_rules(*grid, 0, i, 1) ||    /* row conflict */
            sudoku_errors_rules(*grid, 0, i, 2)) {    /* block conflict */
                grid_clear_rulesok(grid);
                return;
        }
    }
    grid_set_rulesok(grid);
    return;
}


/* sudoku_try_next

Finds a sudoku cell that has the minimum number of available choices among all
cells.

Checks: if row and col are NULL.

Parameters:
grid: a Grid_T type.
row: pointer to a row index.
col: pointer to a column index.

Returns: *row and *col are set to the row, col of the cell that has the
minimum number of choices among all cells. Returns one of its available
choices (1 to 9) or 0 if such cell does not exist */
static int sudoku_try_next(Grid_T grid, int *row, int *col) {
    int rowi, colj, val, choices, min_choices, scanned_cells, filled_cells;

    assert(row);
    assert(col);
    *row = -1;
    *col = -1;
    scanned_cells = 0;
    filled_cells = 0;
    min_choices = SIZE+1;

    /* Pick a random cell (rowi, colj) and scan grid horizontally starting
    from that cell */
    rowi = rand() % SIZE;
    colj = rand() % SIZE;
    while (scanned_cells != SIZE*SIZE) {
        while (colj != SIZE && scanned_cells != SIZE*SIZE) {
            val = grid_read_value(grid, rowi, colj);

            /* skip filled-in cells */
            if (val) {
                filled_cells++;
                colj++;
                scanned_cells++;
                continue;
            }

            /* puzzle is invalid if there is a cell with 0 choices */
            choices = grid_read_count(grid, rowi, colj);
            if (choices == 0) {
                return 0;
            }

            /* update min number of choices */
            if (choices < min_choices) {
                *row = rowi;
                *col = colj;
                min_choices = choices;
            }
            colj++;
            scanned_cells++;
        }
        rowi++;
        if (rowi == SIZE) {
            rowi = 0;
        }
        colj = 0;
    }

    /* there is no cell with a minimum number of choices if all cells are
    filled-in */
    if (filled_cells == SIZE*SIZE) {
        return 0;
    }

    /* we have now selected a cell, return one of its available choices */
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


/* sudoku_set_choice

Sets val as the value of the given sudoku at index (row, col) and removes val
from the available choices of every cell in the same row, column, and
corresponding block.

Checks: if grid is NULL.
        if 0 <= row < 9.
        if 0 <= col < 9.
        if 1 <= val <= 9.

Parameters:
grid: a pointer to a Grid_T type.
row: row index.
col: column index.
val: the inserted value.

Returns: void */
static void sudoku_set_choice(Grid_T *grid, int row, int col, int val) {
    int i, choice, brow, bcol;

    assert(grid);
    assert(row >= 0 && row < SIZE);
    assert(col >= 0 && col < SIZE);
    assert(val >= 1 && val <= SIZE);
    grid_update_value(grid, row, col, val);

    /* set the number of choices to 0 and clear all choices */
    grid_clear_count(grid, row, col);
    for (choice = 1; choice <= SIZE; choice++) {
        grid_clear_choice(grid, row, col, choice);
    }

    /* remove val as a choice from all cells in the same row, col */
    for (i = 0; i < SIZE; i++) {
        grid_remove_choice(grid, row, i, val);
        grid_remove_choice(grid, i, col, val);
    }

    /* remove val as a choice from all cells that belong to the block
    that contains (row, col) */
    for (brow = SUBB(row); brow < SUBB(row) + BSIZE; brow++) {
        for (bcol = SUBB(col); bcol < SUBB(col) + BSIZE; bcol++) {
            grid_remove_choice(grid, brow, bcol, val);
        }
    }
    return;
}


/* sudoku_insert_value

Sets val as the value of the given sudoku at index (row, col).

Checks: if grid is NULL.
        if 0 <= row < 9.
        if 0 <= col < 9.
        if 1 <= val <= 9.

Parameters:
grid: a pointer to a Grid_T type.
row: row index.
col: column index.
val: the inserted value.

Returns: void */
void sudoku_insert_value(Grid_T *grid, int row, int col, int val) {
    assert(grid);
    assert(row >= 0 && row < SIZE);
    assert(col >= 0 && col < SIZE);
    assert(val >= 1 && val <= SIZE);
    grid_update_value(grid, row, col, val);
    return;
}


/* sudoku_delete_value

Deletes the value of the given sudoku at index (row, col).

Checks: if grid is NULL.
        if 0 <= row < 9.
        if 0 <= col < 9.

Parameters:
grid: a pointer to a Grid_T type.
row: row index.
col: column index.

Returns: void */
void sudoku_delete_value(Grid_T *grid, int row, int col) {
    assert(grid);
    assert(row >= 0 && row < SIZE);
    assert(col >= 0 && col < SIZE);
    grid_update_value(grid, row, col, 0);
    return;
}


/* sudoku_generate_complete

Generates a random valid sudoku with no empty cells.

Parameters: void

Returns: a Grid_T struct */
static Grid_T sudoku_generate_complete(void) {
    int choice, row, col, val, tries;
    Grid_T sudoku;

    grid_set_unique(&sudoku);
    grid_set_rulesok(&sudoku);
    grid_clear_initialized(&sudoku);
    grid_set_formatok(&sudoku);
    
    /* try to generate a full puzzle 20 times */
    for (tries = 0; tries < 20; tries++) {

        /* start with an empty puzzle and initialize cell choices */
        for (row = 0; row < SIZE; row++) {
            for (col = 0; col < SIZE; col++) {
                grid_update_value(&sudoku, row, col, 0);
                grid_set_count(&sudoku, row, col);
                grid_clear_choice(&sudoku, row, col, 0);
                for (choice = 1; choice <= SIZE; choice++) {
                    grid_set_choice(&sudoku, row, col, choice);
                }
            }
        }

        /* fill random cells starting from the ones that have the min number
        of choices */
        while((val = sudoku_try_next(sudoku, &row, &col))) {
            sudoku_set_choice(&sudoku, row, col, val);
        }

        if (sudoku_is_correct(sudoku, 0)) {
            return sudoku;
        }
    }

    /* if failed 20 times to generate a puzzle, return the one below */
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
2) Clear a random cell and check whether the puzzle has a unique choice
solution.
3) If yes, repeat previous step. Return when the nelts non-zero requirements
is met.
3) If no, pick another cell. If all cells have been tried and all of them
lead to a non unique choice solution, continue clearing cells until the
nelts non-zero cells requirement is met.

We can find whether the returned puzzle has a unique choice solution by
calling sudoku_solution_is_unique() before solving the puzzle.

Parameters:
nelts: number of non-zero cells.

Returns: a Grid_T type */
Grid_T sudoku_generate(int nelts) {
    int to_remove, row, col, val, has_nontried, unique_choice;
    int tried[SIZE][SIZE];
    Grid_T sudoku, sudoku_copy, sudoku_solved;

    assert(nelts >= 0 && nelts <= SIZE * SIZE);
    to_remove = SIZE * SIZE - nelts;
    has_nontried = 0;
    unique_choice = 1;

    /* Initialize tried to 0. Set tried(i,j) to 1 if clearing cell (i, j)
    did not lead to a unique choice solution.  */
    for (row = 0; row < SIZE; row++) {
        for (col = 0; col < SIZE; col++) {
            tried[row][col] = 0;
        }
    }

    /* generate a random solved puzzle */
    sudoku = sudoku_generate_complete();

    while (to_remove > 0) {

        /* pick cell (row, col) and proceed only if it still has a value */
        row = rand() % SIZE;
        col = rand() % SIZE;
        if (!grid_read_value(sudoku, row, col)) {
            continue;
        }

        /* if puzzle cannot have a unique choice solution we only need to
        clear the cell value and pick another cell */
        if (!unique_choice) {
            to_remove--;
            grid_update_value(&sudoku, row, col, 0);
            continue;
        }

        /* if puzzle still has a unique choice solution, we need to
        pick a different cell if this one has been tried */
        if (tried[row][col]) {
            continue;
        }

        /* initialize a copy of puzzle and clear the cell value on the copy */
        sudoku_copy = sudoku;
        grid_update_value(&sudoku_copy, row, col, 0);
        sudoku_init_choices(&sudoku_copy);

        /* solve the copy and if there is a unique choice solution,
        we'll be using the copy from now on */
        sudoku_solved = sudoku_solve(sudoku_copy);
        if (grid_read_unique(sudoku_solved)) {
            to_remove--;
            sudoku = sudoku_copy;
            for (row = 0; row < SIZE; row++) {
                for (col = 0; col < SIZE; col++) {
                    tried[row][col] = 0;
                }
            }
            continue;
        }

        /* if solving the copy did not lead to unique choice solution,
        set the cell to tried */
        tried[row][col] = 1;

        /* and search for a new non-tried cell */
        has_nontried = 0;
        for (row = 0; row < SIZE; row++) {
            for (col = 0; col < SIZE; col++) {
                val = grid_read_value(sudoku, row, col);
                if (val && !tried[row][col]) {
                    has_nontried = 1;
                }
            }
        }

        /* if a non-tried cell cannot be found, clearing a cell
        creates a puzzle that will not have unique choice solution */
        if (!has_nontried) {
            unique_choice = 0;
        }
    }

    if (!unique_choice) {
        grid_clear_unique(&sudoku);
    }
    return sudoku;
}


/* sudoku_solve

Solves the given sudoku using recursion (backtracking).

- If there are multiple solutions, it returns one of them.
- If there is no solution, it returns a puzzle that is as close as possible
to a solution.
- If the puzzle violates a rule, it returns the initial puzzle.

Parameters:
grid: a Grid_T type

Returns: a Grid_T type */
Grid_T sudoku_solve(Grid_T grid) {
    int row, col, val;
    Grid_T grid_copy;

    /* if puzzle violates any rule */
    if (grid_read_rulesok(grid) == -1) {
        sudoku_set_rules(&grid);
    }

    /* if grid violates sudoku rules */
    if (!grid_read_rulesok(grid)) {
        grid_clear_unique(&grid);
        return grid;
    }

    /* initialize cell choices once */
    if (!grid_is_initialized(grid)) {
        sudoku_init_choices(&grid);
        grid_set_initialized(&grid);
    }

    /* find a cell with min number of choices among all cells */
    while ((val = sudoku_try_next(grid, &row, &col))) {

        /* if there is a cell that has only 1 choice, fill it */
        if (grid_read_count(grid, row, col) == 1) {
            sudoku_set_choice(&grid, row, col, val);
        }

        /* else puzzle does not have a unique choice solution. solve a copy */
        else {
            grid_clear_unique(&grid);
            grid_copy = grid;
            sudoku_set_choice(&grid_copy, row, col, val);
            grid_copy = sudoku_solve(grid_copy);

            /* return the copy if solution is correct */
            if (sudoku_is_correct(grid_copy, 0)) {
                return grid_copy;
            }

            /* else remove the choice from the cell of the original puzzle */
            else {
                grid_remove_choice(&grid, row, col, val);
            }
        }
    }

    /* clear unique flag if puzzle not correct */
    if (grid_read_unique(grid) && !sudoku_is_correct(grid, 0)) {
        grid_clear_unique(&grid);
    }
    return grid;
}


/* sudoku_has_unique_choice_solution

Indicates whether the given sudoku has a unique choice solution.

Parameters:
grid: a Grid_T type

Returns:
1 if grid has a unique choice solution
0 if grid does not have a unique choice solution
-1 if it is not known whether grid has a unique choice solution */
int sudoku_has_unique_choice_solution(Grid_T grid) {
    return grid_read_unique(grid);
}
