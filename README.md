# Sudoku library

A solver/generator library for [Sudoku](https://en.wikipedia.org/wiki/Sudoku) puzzles.

The following functions are provided:

* sudoku_read(): Read a puzzle
* sudoku_print(puzzle): Print a puzzle
* sudoku_print_errors(puzzle): Print all puzzle errors including empty cells
* sudoku_is_correct(puzzle): Check whether a puzzle is fully completed and does not have any errors
* sudoku_solve(puzzle): Solve a puzzle
* sudoku_generate(N): Generate puzzle that has N non empty cells
* sudoku_solution_is_unique(puzzle): Show whether a puzzle has a unique choice solution

## Implementation

Sudokus are solved using a [backtracking](https://en.wikipedia.org/wiki/Backtracking) algorithm.

## Profiling

The program has been tested for memory leaks with [AddressSanitizer](https://github.com/google/sanitizers/wiki/AddressSanitizer).

## Compile

* Build the library (functions declared in sudoku.h):

```bash
make sudoku.o
```

* Build the UI that uses the library:

```bash
make sudoku-ui
```

## Typical usage

* Read a sudoku from input_file and attemp to solve it:

```bash
./sudoku-ui < input_file
```

* Read a sudoku from input_file and check its correctness:

```bash
./sudoku-ui -c < input_file
```

* Read a sudoku from input_file and print it in a 9x9 grid:

```bash
./sudoku-ui -s < input_file
```

* Generate a unique choice solvable sudoku with 40 non-zero numbers:

```bash
./sudoku-ui -g 40
```

Note: If N is very low, a unique choice puzzle may not be possible. In that case the program returns a
puzzle where at each step multiple cells can be filled in.

* Generate a unique choice solvable sudoku with 40 non-zero numbers and solve it:

```bash
./sudoku-ui -g 40 | ./sudoku
```

Note: If the puzzle has multiple solutions, one of them is returned.

## Example puzzles

[Puzzles](puzzles/) folder contains a selection of unsolved puzzles.
