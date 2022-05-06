CFLAGS = -c -ansi -Wall -pedantic

sudoku-ui: sudoku-ui.o sudoku.o grid.o
	gcc sudoku-ui.o sudoku.o grid.o -o sudoku-ui

sudoku-ui.o: sudoku-ui.c sudoku.h grid.h
	gcc $(CFLAGS) sudoku-ui.c

sudoku.o: sudoku.c sudoku.h grid.h
	gcc $(CFLAGS) sudoku.c

grid.o: grid.c grid.h
	gcc $(CFLAGS) grid.c

clean:
	rm -f *.o sudoku-ui
