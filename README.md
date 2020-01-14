# Sudoku

## Description

[Sudoku](https://en.wikipedia.org/wiki/Sudoku) solver/generator.

Note: The program solves sudokus using the [Backtracking algorithm](https://en.wikipedia.org/wiki/Backtracking). No other algorithms or shortcuts are employed.

## Compile

The program is written in C.

To compile, use the provided Makefile:

```bash
make sudoku
```

## Typical usage

* Read a sudoku from input_file and solve it if possible:

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

Such puzzles can be solved easily because at each step there is always one cell with only once choice left.

Note: If N is very low, a unique choice puzzle may not be possible. In that case the program returns a
puzzle where at each step multiple cells can be filled in.

* Generate a unique choice solvable sudoku with 40 non-zero numbers and solve it:

```bash
./sudoku -g 40 | ./sudoku
```

Note: If the puzzle has multiple solutions, the program returns one of them.

## Sample files

See [tests](tests) folder.
