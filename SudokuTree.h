

#ifndef EX3_SUDOKUTREE_H
#define EX3_SUDOKUTREE_H
#define MEMORY_ERROR -1
#define INPUT_ERROR -2
#define NO_SOLUTION_EXIT -3

#include "GenericDFS.h"
#endif //EX3_SUDOKUTREE_H

/**
 * this structs represents a sudoku table
 */
typedef struct
{
    int** board;
    int size;
} sudoku;

/**
 * receives a current sudoku board, and updated an array of boards to contain all possible
 * options for the next solution step
 * @param root - the current sudoku board
 * @param arrOfOptions - an updating array of possible steps
 * @return the number of possible next steps
 */
int getSudokuOptions(void* root, void*** arrOfOptions);

/**
 * this method counts the number of solved cells in a given sudoku board and returns it
 * @param board - a sudoku board to check
 * @return - number of solved cells in the given sudoku board
 */
unsigned int numOfSolvedCells(void*);

/**
 * this function receives a pointer to sudoku board and frees it's allocated memory
 * @param board - the given sudoku board pointer to free
 */
void freeBoard(void*);

/**
 * creates a copy of a given sudoku struct
 * @param sudokuBoard - a struct of a sudoku board to copy
 * @return - a sudoku pointer which is a copy of the given sudoku struct
 */
void* copyBoard(void*);

/**
 * creates a new board with the given dimension
 * @param dimension - the dimension of the board
 * @return a pointer to the new created board
 */
sudoku* createBoard (int);