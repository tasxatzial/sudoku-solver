# Sudoku

## Description

[Sudoku](https://en.wikipedia.org/wiki/Sudoku) solver/generator library.

The following functions are provided:

* sudoku_read(): Read a puzzle
* sudoku_print(puzzle): Print a puzzle
* sudoku_print_errors(puzzle): Print all puzzle errors including empty cells
* sudoku_is_correct(puzzle): Check whether a puzzle is fully completed and does not have any errors
* sudoku_solve(puzzle): Solve a puzzle
* sudoku_generate(puzzle, N): Generate puzzle that has N non empty cells
* sudoku_solution_is_unique(puzzle): Show whether a puzzle has a unique solution

## Implementation

Sudokus are solved using the Backtracking algorithm.

## Compile

Use the provided Makefile:

### Building the library

```bash
make puzzle.o
```

### Building the test files

```bash
make sudoku
```

Running the tests is explained in the next section.

## Typical usage

* Read a sudoku from input_file and if possible, solve it:

```bash
./sudoku < input_file
```

* Read a sudoku from input_file and check its correctness:

```bash
./sudoku -c < input_file
```

* Read a sudoku from input_file and print it in a 9x9 grid:

```bash
./sudoku -s < input_file
```

* Generate a unique choice solvable sudoku with 40 non-zero numbers:

```bash
./sudoku -g 40
```

Note: If N is very low, a unique choice puzzle may not be possible. In that case the program returns a
puzzle where at each step multiple cells can be filled in. This, however, makes the puzzle much harder to
solve (if you are a human).

* Generate a unique choice solvable sudoku with 40 non-zero numbers and solve it:

```bash
./sudoku -g 40 | ./sudoku
```

Note: If the puzzle has multiple solutions, one of them is returned.

## Sample files

See [tests](tests).
