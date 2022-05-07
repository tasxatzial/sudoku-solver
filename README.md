# Sudoku library

Library for generating and solving [Sudoku](https://en.wikipedia.org/wiki/Sudoku) puzzles.

The following functions are provided:

* sudoku_read(): Read a puzzle
* sudoku_print(puzzle): Print puzzle
* sudoku_print_errors(puzzle): Print puzzle issues (rules violation, empty cells)
* sudoku_format_is_correct(puzzle): Show whether puzzle meets the required format
* sudoku_is_correct(puzzle): Check whether puzzle has issues (rules violation, empty cells)
* sudoku_solve(puzzle): Solve puzzle
* sudoku_generate(N): Generate puzzle that has N non empty cells
* sudoku_has_unique_choice_solution(puzzle): Show whether puzzle has a unique choice solution

## Implementation

Sudokus are solved using a [backtracking](https://en.wikipedia.org/wiki/Backtracking) algorithm.

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

* Read a sudoku from input_file and solve it:

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

* Generate a solvable sudoku with 40 non-zero numbers:

```bash
./sudoku-ui -g 40
```

* Generate a solvable sudoku with 40 non-zero numbers and solve it:

```bash
./sudoku-ui -g 40 | ./sudoku-ui
```

Note: When the non-zero count is very low, it may not be possible to return a puzzle with a unique available choice at each step of the solution. Consequently, such puzzles may have multiple solutions. In such cases, the solver will provide only one of the possible solutions.

## Puzzles

[Puzzles](puzzles/) folder contains a selection of unsolved puzzles.

## Profiling

'sudoku-ui' has been tested for memory leaks with [valgrind](https://valgrind.org/) and [AddressSanitizer](https://github.com/google/sanitizers/wiki/AddressSanitizer).
