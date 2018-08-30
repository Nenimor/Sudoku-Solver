CC= gcc
CFLAGS= -c -Wvla -Wall -Wextra

all: libGenericDFS.a SudokuSolver


# Exceutables

libGenericDFS.a: GenericDFS.o
	ar rcs libGenericDFS.a GenericDFS.o

SudokuSolver: SudokuSolver.o SudokuTree.o libGenericDFS.a
	$(CC) -DNDEBUG  SudokuSolver.o SudokuTree.o -lm -L. -lGenericDFS -o SudokuSolver

# Object Files

GenericDFS.o: GenericDFS.h GenericDFS.c
	$(CC) $(CFLAGS) -DNDEBUG -std=c99 GenericDFS.c -o GenericDFS.o

SudokuSolver.o: SudokuSolver.c SudokuTree.h GenericDFS.h
	$(CC) $(CFLAGS) -DNDEBUG -std=c99 SudokuSolver.c -o SudokuSolver.o

SudokuTree.o: SudokuTree.c SudokuTree.h
	$(CC) $(CFLAGS) -DNDEBUG -std=c99 SudokuTree.c -o SudokuTree.o

# Other

clean:
	-rm -f *.o SudokuSolver


