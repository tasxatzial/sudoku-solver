/* Sudoku generator/solver library */

#ifndef _PUZZLE_H_
#define _PUZZLE_H_

#include <stdio.h>
#include "grid.h"


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

Returns: a Grid_T type */
Grid_T sudoku_read(void);


/* sudoku_format_is_correct

Checks if the Grid_T returned by sudoku_read() has the correct format.

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

Solves grid using recursion (backtracking method). If there is more than
one solution, it returns one of them. If there is no solution, it returns one
of the attempted solutions. If the puzzle violates a sudoku rule, it returns
the initial puzzle.

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
Grid_T sudoku_generate(int nelts);


#endif