#include "GenericDFS.h"
#include "SudokuTree.h"
#include <math.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#define UNSOLVED_CELL 0

/**
 * creates a new board with the given dimension
 * @param dimension - the dimension of the board
 * @return a pointer to the new created board
 */
sudoku* createBoard (int dimension)
{
    assert(dimension > 0);
    sudoku* newBoard = (sudoku*)malloc(sizeof(sudoku));
    if(newBoard == NULL)
    {
        return NULL;
    }
    newBoard->size = dimension;
    //allocates memory to the board
    newBoard->board = (int**)malloc(dimension*sizeof(int*));
    if (newBoard->board == NULL)
    {
        free(newBoard);
        newBoard = NULL;
        return NULL;
    }
    //allocates memory to each row
    for (int i = 0; i < dimension; ++i)
    {
        newBoard->board[i] = (int*)malloc(dimension* sizeof(int));
        if (newBoard->board[i] == NULL)
        {
            //updates the size of the board to be i (num of initialized rows), for freeboard method
            newBoard->size = i;
            freeBoard(newBoard);
            return NULL;
        }
    }
    return newBoard;
}

/**
 * creates a copy of a given sudoku struct
 * @param sudokuBoard - a struct of a sudoku board to copy
 * @return - a sudoku pointer which is a copy of the given sudoku struct
 */
void* copyBoard(void* sudokuBoard)
{
    sudoku* currentBoard = (sudoku*)sudokuBoard;
    sudoku* sudokuCopy = createBoard(currentBoard->size);
    if (sudokuCopy == NULL) // failed to create board
    {
        return NULL;
    }
    for (int i = 0; i < currentBoard->size ; ++i)
    {
        for (int j = 0; j < currentBoard->size ; ++j)
        {
            sudokuCopy->board[i][j] = currentBoard->board[i][j];
        }
    }
    return sudokuCopy;
}

/**
 * eliminates cell's possibilities in its box
 * @param currentBoard
 * @param indicatorArr
 * @param rowIndex
 * @param colIndex
 */
void checkBoxOptions(sudoku* currentBoard, int* indicatorArr, int rowIndex, int colIndex)
{
    int rowStartIdx, colStartIdx;
    int boxSize = (int)sqrt((double)currentBoard->size);
    rowStartIdx = rowIndex / boxSize;
    rowStartIdx *= boxSize;
    colStartIdx = colIndex / boxSize;
    colStartIdx *= boxSize;
    for (int i = rowStartIdx ; i < rowStartIdx + boxSize ; ++i)
    {
        for (int j = colStartIdx ; j < colStartIdx + boxSize ; ++j)
        {
            if (currentBoard->board[i][j] != UNSOLVED_CELL)
            {
                indicatorArr[currentBoard->board[i][j]] = 1;
            }
        }
    }
}

/**
 * eliminates cell's possibilities in its row, column and box
 * @param currentBoard -
 * @param indicatorArr - array of (currentBoard.size+1) ints, initialized with 0's
 * @param rowIndex
 * @param colIndex
 */
void findCellOptions(sudoku* currentBoard, int* indicatorArr, int rowIndex, int colIndex)
{
    //check possibilities in the row & column of the cell
    for (int i = 0; i < currentBoard->size ; ++i)
    {
        if (currentBoard->board[rowIndex][i] != UNSOLVED_CELL )
        {
            indicatorArr[currentBoard->board[rowIndex][i]] = 1;
        }
        if (currentBoard->board[i][colIndex] != UNSOLVED_CELL )
        {
            indicatorArr[currentBoard->board[i][colIndex]] = 1;
        }
    }

    //check possibilities in the box of the cell
    checkBoxOptions(currentBoard, indicatorArr, rowIndex, colIndex);
}

/**
 * finds the first unsolved cell in the sudoku board
 * @param currentBoard the sudoku board to check
 * @param rowIndex will hold the index of the first unsolved cell row
 * @param colIndex will hold the index of the first unsolved cell column
 */
void findUnsolvedIndex(sudoku* currentBoard, int* rowIndex, int* colIndex)
{
    for (int i = 0; i < currentBoard->size; i++)
    {
        for (int j = 0; j < currentBoard->size; j++)
        {
            if (currentBoard->board[i][j] == UNSOLVED_CELL)
            {
                *rowIndex = i;
                *colIndex = j;
                return;
            }
        }
    }
}

/**
 * receives a current sudoku board, and updated an array of boards to contain all possible
 * options for the next solution step
 * @param root - the current sudoku board
 * @param arrOfOptions - an updating array of possible steps
 * @return the number of possible next steps
 */
int getSudokuOptions(void* root, void*** arrOfOptions)
{
    sudoku ***optionsArray = (sudoku***) arrOfOptions;
    sudoku* currentBoard = (sudoku*)root;
    *optionsArray = (sudoku**)malloc(currentBoard->size*(sizeof(sudoku*)));
    if(*optionsArray == NULL)
    {
        return MEMORY_ERROR;
    }

    int numOfOptions = 0;
    //initializes indicators array with 0's
    int* possibilitiesArr = (int*)malloc((currentBoard->size + 1)* sizeof(int));
    if (possibilitiesArr == NULL)
    {
        free(*optionsArray);
        return MEMORY_ERROR;
    }
    for (int j = 0; j < currentBoard->size + 1 ; ++j)
    {
        possibilitiesArr[j] = 0;
    }

    //finds first empty (unsolved) cell, and keep it's indexes
    int rowIndex = 0, colIndex = 0;
    findUnsolvedIndex(currentBoard, &rowIndex, &colIndex);

    //builds the indicators array
    findCellOptions(currentBoard, possibilitiesArr, rowIndex, colIndex);

    //creates a struct for each possible next step, and appends it to arrOfOptions
    for (int i = 1; i < currentBoard->size + 1 ; i++)
    {
        if (possibilitiesArr[i] == 0)
        {
            sudoku *newBoard;
            newBoard = copyBoard(currentBoard);
            if(newBoard == NULL) //
            {
                for (int j = 0 ; j < numOfOptions; ++j)
                {
                    freeBoard((*optionsArray)[j]);
                }
                free(*optionsArray);
                free(possibilitiesArr);
                (*optionsArray) = NULL;
                possibilitiesArr = NULL;
                return MEMORY_ERROR;
            }
            newBoard->board[rowIndex][colIndex] = i;
            (*optionsArray)[numOfOptions] = newBoard;
            numOfOptions++;
        }
    }
    free(possibilitiesArr);
    return numOfOptions;
}

/**
 * this method counts the number of solved cells in a given sudoku board and returns it
 * @param board - a sudoku board to check
 * @return - number of solved cells in the given sudoku board
 */
unsigned int numOfSolvedCells(void* board)
{
    unsigned int value = 0;
    sudoku* checkSudoku = (sudoku*)board;
    for (int i = 0; i < checkSudoku->size ; ++i)
    {
        for (int j = 0; j < checkSudoku->size; ++j)
        {
            if (checkSudoku->board[i][j] != UNSOLVED_CELL)
            {
                value++;
            }
        }
    }
    return value;
}

/**
 * this function receives a pointer to sudoku board and frees it's allocated memory
 * @param board - the given sudoku board pointer to free
 */
void freeBoard(void* board)
{
    sudoku* boardToFree = (sudoku*)board;

    for (int j = 0; j < boardToFree->size ; ++j)
    {
        free(boardToFree->board[j]);
        boardToFree->board[j] = NULL;
    }
    free(boardToFree->board);
    boardToFree->board = NULL;
    free(boardToFree);
    boardToFree = NULL;
}
