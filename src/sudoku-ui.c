/* Test file for the sudoku library */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "sudoku.h"

int checkarg(int argc, char **argv);


/* main

Parameters:
argc: number of command line arguments
argv: the command line arguments

If run with 0 arguments:
- Reads a puzzle from stdin.
- Prints the puzzle to stderr.
- Solves the puzzle.
- Prints a message to stderr indicating if the puzzle has a unique choice
solution, multiple choices solutions, or no solution.
- If a solution exists, it prints it to stderr.
- If a solutions does not exist, it prints one of the puzzles that
have been examined and its errors.

If run with argument '-c':
- Reads a puzzle from stdin.
- Prints the puzzle to stderr.
- Checks if the puzzle is correct (fully completed and does not violate any 
rules)
- Prints a message to stderr indicating if the puzzle is correct or not.
- If the puzzle is not correct, it prints its errors.

If run with argument '-g <nelts>':
- Generates a random puzzle with ~nelts (1 <= nelts <= 81) completed cells and
prints it to stdout.

If run with argument '-h':
- Prints the help message to stderr.

If run with argument '-s':
- Reads a puzzle from stdin and prints it to stdout in the standard 9x9 grid
shown in puzzle.h

Any combination of the above arguments will be rejected and the help message
will be displayed. */
int main(int argc, char **argv) {
    Grid_T sudoku, sudoku_solved;
    int nelts;

    srand(getpid());
    if (argc == 1) {        /* no arguments */
        sudoku = sudoku_read();
        if (!sudoku_format_is_correct(sudoku)) {
            fprintf(stdout, "Puzzle has incorrect format. Exiting...\n");
            return 0;
        }
        fprintf(stderr, "Input puzzle:\n");
        sudoku_print(sudoku);
        if (!sudoku_is_correct(sudoku, 1)) {
            fprintf(stdout, "Puzzle violates rules:\n");
            sudoku_print_errors(sudoku, 1);
            return 0;
        }
        sudoku_solved = sudoku_solve(sudoku);
        if (!sudoku_is_correct(sudoku_solved, 0)) {
            fprintf(stdout, "Puzzle has no solutions\n");
            return 0;
        }
        if (sudoku_solution_is_unique(sudoku_solved)) {
            fprintf(stderr, "Puzzle has one (unique choice) solution:\n");
        }
        else {
            fprintf(stderr, "Puzzle has a solution (multiple solutions may exist):\n");
        }
        sudoku_print(sudoku_solved);
    }
    else if (argc == 3) {   /* argument -g <nelts> */
        if (checkarg(argc, argv)) {
            return 0;
        }
        nelts = atoi(argv[2]);
        sudoku = sudoku_generate(nelts);
        if (sudoku_solution_is_unique(sudoku)) {
            fprintf(stderr, "Generating (unique choice) solvable puzzle...\n");
        }
        else {
            fprintf(stderr, "Generating solvable puzzle (multiple solutions may exist)...\n");
        }
        sudoku_print(sudoku);
    }
    else if (argc == 2) {   /* argument -c, -s, -h */
        if (checkarg(argc, argv)) { /* -h */
            return 0;
        }
        sudoku = sudoku_read();
        sudoku_print(sudoku);
        if (!strcmp(argv[1], "-s")) { /* -s */
            return 0;
        }
        if (!sudoku_is_correct(sudoku, 0)) {
            fprintf(stderr, "Issues found:\n");
            sudoku_print_errors(sudoku, 0);
        }
        else {
             fprintf(stderr, "No issues found\n");
        }
    }
    return 0;
}


/* checkarg

Checks that the command line arguments indicated by the parameters argc and
argv have the expected format and prints a help message. Only
the following arguments are considered valid:
-h
-g <nelts>  (1 <= nelts <= 81)
-c
-s
Any combination of the above arguments will be rejected and the help message
will be displayed.

Parameters:
argc: number of command line arguments
argv: the command line arguments

Returns: 0 if the arguments are valid, 0 otherwise. */
int checkarg(int argc, char **argv) {
    int err = 0;

    if (argc == 3) {
        if (strcmp(argv[1], "-g")) {
            err = 1;
        }
        if (!atoi(argv[2])) {
            err = 1;
        }
    }
    else if (argc == 2) {
        if (strcmp(argv[1], "-c") && strcmp(argv[1], "-s")) {
            err = 1;
        }
        else if (!strcmp(argv[1], "-h")) {
            err = 1;
        }
    }
    else {
        err = 1;
    }
    if (err) {
        fprintf(stderr, "Usage: %s [option]\n", argv[0]);
        fprintf(stderr, "Options (only one option can be used at a time):\n");
        fprintf(stderr, "  -h\t\tDisplay this information\n");
        fprintf(stderr, "  -c\t\tReads a sudoku puzzle from stdin");
        fprintf(stderr, " and checks its correctness\n");
        fprintf(stderr, "  -s\t\tReads a sudoku puzzle from stdin");
        fprintf(stderr, " and prints it to stdout\n");
        fprintf(stderr, "  -g <number>\tGenerates a sudoku puzzle");
        fprintf(stderr, " with approximately <number> completed cells\n");
    }
    return err;
}
