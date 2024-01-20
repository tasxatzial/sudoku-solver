/* Sudoku generator/solver library */

#ifndef _PUZZLE_H_
#define _PUZZLE_H_

#include <stdio.h>
#include "grid.h"


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
Grid_T sudoku_read(void);


/* sudoku_format_is_correct

Indicates whether the given sudoku has the correct format.

Parameters:
grid: a Grid_T type.

Returns: 1 if the format is OK, 0 otherwise */
int sudoku_format_is_correct(Grid_T grid);


/* sudoku_print:

Writes the given sudoku to stdout.

The format is 9 numbers per line. After each number there is a space char.
Empty cells are denoted by a dot char. After the last cell in each line there
is a LF char.

Parameters:
grid: a Grid_T type.

Returns: void */
void sudoku_print(Grid_T grid);


/* sudoku_print_errors

Writes to stdout all rule-related errors of the given sudoku.

Parameters:
grid: a Grid_T type.

Returns: void */
void sudoku_print_errors(Grid_T grid);


/* sudoku_print_empty

Writes to stdout all empty cells of the given sudoku.

Parameters:
grid: a Grid_T type.

Returns: void */
void sudoku_print_empty(Grid_T grid);


/* sudoku_is_correct

Checks if the given sudoku does not violate any rule. The additional
parameter allow_empty controls whether we would want to consider empty
cells as errors.

Parameters:
grid: a Grid_T type.
allow_empty: A value of zero will only check for the standard rules violations,
any other value will also consider empty cells as errors.

Returns:
if allow_empty != 0: 1 if puzzle is completed and does not violate rules, else 0.
if allow_empty = 0: 1 if puzzle does not violate rules, else 0. */
int sudoku_is_correct(Grid_T grid, int allow_empty);


/* sudoku_solve

Solves the given sudoku using recursion (backtracking).

- If there are multiple solutions, it returns one of them.
- If there is no solution, it returns a puzzle that is as close as possible
to a solution.
- If the puzzle violates a rule, it returns the initial puzzle.

Parameters:
grid: a Grid_T type

Returns: a Grid_T type */
Grid_T sudoku_solve(Grid_T grid);


/* sudoku_has_unique_choice_solution

Indicates whether the given sudoku has a unique choice solution.

Parameters:
grid: a Grid_T type

Returns:
1 if grid has a unique choice solution
0 if grid does not have a unique choice solution
-1 if it is not known whether grid has a unique choice solution */
int sudoku_has_unique_choice_solution(Grid_T grid);


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
Grid_T sudoku_generate(int nelts);


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
void sudoku_insert_value(Grid_T *grid, int row, int col, int val);


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
void sudoku_delete_value(Grid_T *grid, int row, int col);


#endif