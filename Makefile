CFLAGS = -c -ansi -Wall -pedantic

sudoku: main.o puzzle.o grid.o
	gcc main.o puzzle.o grid.o -o sudoku

main.o: main.c puzzle.h
	gcc $(CFLAGS) main.c

puzzle.o: puzzle.c puzzle.h grid.h
	gcc $(CFLAGS) puzzle.c

grid.o: grid.c grid.h
	gcc $(CFLAGS) grid.c

clean:
	rm -f *.o sudoku