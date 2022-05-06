/* Test file for the sudoku library */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "sudoku.h"

int checkarg(int argc, char **argv);


/* main

Use:

1) Read a sudoku form stdin and attempt to solve it:
./sudoku-ui

2) Read a sudoku from input_file and attempt to solve it:
./sudoku-ui < input_file

3) Read a sudoku from input_file and check its correctness:
./sudoku-ui -c < input_file

4) Read a sudoku from input_file and print it in a 9x9 grid:
./sudoku-ui -s < input_file

5) Generate a solvable sudoku with 40 non-zero numbers:
./sudoku-ui -g 40

6) Generate a solvable sudoku with 40 non-zero numbers and solve it:
./sudoku-ui -g 40 | ./sudoku-ui

7) Display the help:
./sudoku-ui -h
*/
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
