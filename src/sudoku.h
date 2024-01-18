/* Sudoku generator/solver library */

#ifndef _PUZZLE_H_
#define _PUZZLE_H_

#include <stdio.h>
#include "grid.h"


/* sudoku_read: Reads a sudoku puzzle from stdin.

The input should have the format:
1 . . . . 7 . 9 .
. 3 . . 2 . . . 8
. . 9 6 . . 5 . .
. . 5 3 . . 9 . .
. 1 . . 8 . . . 2
6 . . . . 4 . . .
3 . . . . . . 1 .
. 4 . . . . . . 7
. . 7 . . . 3 . .

Between each value there should be a space char. New lines are denoted by a
LF char. A dot char indicates an empty cell.

Parameters: void

Returns: a Grid_T type. */
Grid_T sudoku_read(void);


/* sudoku_format_is_correct

Indicates whether grid has the correct format.

Parameters:
grid: a Grid_T type.

Returns: 1 if the format is OK, 0 otherwise */
int sudoku_format_is_correct(Grid_T grid);


/* sudoku_print: Writes a sudoku puzzle grid to stdout.

The format is 9 numbers per line and after each number there is a space char.
After the last number in each line there is a LF char.

Parameters:
grid: a Grid_T type.

Returns: void */
void sudoku_print(Grid_T grid);


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
void sudoku_print_errors(Grid_T grid, int rules_only);


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
int sudoku_is_correct(Grid_T grid, int rules_only);


/* sudoku_solve

Solves grid using recursion (backtracking).

- If there ae multiple solutions, it returns one of them.
- If there is no solution, it returns a puzzle that is as close as possible
to a solution.
- If the puzzle violates a sudoku rule, it returns the initial puzzle.

Parameters:
grid: a Grid_T type

Returns: a Grid_T type */
Grid_T sudoku_solve(Grid_T grid);


/* sudoku_has_unique_choice_solution

Indicates whether grid has a unique choice solution.

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

Sets val as the value of (row, col) and removes val from the
available choices of every cell in the same row, column, block.

Checks: if grid is NULL at runtime.

Parameters:
grid: a pointer to a Grid_T type.
row: row index of val.
col: column index of val.
val: the inserted value.

Returns: void */
void sudoku_insert_value(Grid_T *grid, int row, int col, int val);


#endif