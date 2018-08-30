#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "SudokuTree.h"
#include "GenericDFS.h"
#define FILE_ERROR_MSG "%s: no such file\n"
#define INPUT_ERROR_MSG "%s: not a valid sudoku file\n"
#define NO_SOLUTION_MSG "NO SOLUTION!"
#define READ_MAX 300
#define MAX_BOARD_SIZE 100
#define DECIMAL_BASE 10
#define FILE_NAME_IDX 1
#define NUM_OF_ARGUMNETS 2

/**
 * receives a number, and checks whether it's square root is natural or not
 * @param inputToCheck - the given number to check
 * @return 0 if the square root is natural, 1 otherwise
 */
int isSqrtNatural(int inputToCheck)
{
    if ((int)sqrt(inputToCheck) != sqrt(inputToCheck))
    {
        return 1;
    }
    return 0;
}

/**
 * receives a line of parameters, and sudoku board, and updates it accordingly
 * @param boardToUpdate - the given board to update
 * @param parameters - a string of line parameters
 * @param lineIndex - the index of the line to update
 */
int updateBoardWithLine(sudoku* boardToUpdate, char* parameters, int lineIndex)
{
    int newParam;
    for (int i = 0; i < boardToUpdate->size ; ++i)
    {
        newParam = (int)strtol(parameters, &parameters, DECIMAL_BASE);
        if (newParam < 0 || newParam > boardToUpdate->size) // checks input validness
        {
            return EXIT_FAILURE;
        }
        boardToUpdate->board[lineIndex][i] = newParam;
        if (*parameters == ' ')
        {
            parameters++;
        }
        else if (*parameters != '\n')
        {
            if (*parameters != '\0')
            {
                return EXIT_FAILURE;
            }
        }
        else if (*parameters == '\n' && i + 1 != boardToUpdate->size)
        {
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}

/**
 * this function receives an input file, and creates a sudoku board from it
 * @param inputFile - file of a sudoku board parameters
 * @param exitCode - an updating error indicator
 * @return the built sudoku board
 */
sudoku* createBoardFromFile(char *inputFile, int* exitCode)
{
    FILE *file = fopen(inputFile, "r");
    char lineReader[READ_MAX];
    int dimension;

    if (file == NULL)
    {
        fprintf(stderr, FILE_ERROR_MSG, inputFile);
        *exitCode = INPUT_ERROR;
        return NULL;
    }

    fgets(lineReader, READ_MAX, file); //reads first line (N)
    dimension = (int)strtod(lineReader, NULL);

    //checks if N is valid
    if (dimension <= 0 || isSqrtNatural(dimension))
    {
        fprintf(stderr, INPUT_ERROR_MSG, inputFile);
        *exitCode = INPUT_ERROR;
        fclose(file);
        return NULL;
    }
    assert(dimension < MAX_BOARD_SIZE);

    //creates a new board of size dimension X dimnesion
    sudoku* newBoard = createBoard(dimension);
    if (newBoard == NULL)
    {
        *exitCode = MEMORY_ERROR; //not enough memory for the initial board
        fclose(file);
        return NULL;
    }
    for (int lineIndex = 0 ; lineIndex < dimension ; lineIndex++)
    {
        fgets(lineReader, READ_MAX, file); //reads next parameters line (N)
        if (updateBoardWithLine(newBoard, lineReader, lineIndex))
        {
            //failed to read line (bad parameters)
            freeBoard(newBoard);
            fprintf(stderr, INPUT_ERROR_MSG, inputFile);
            *exitCode = INPUT_ERROR;
            fclose(file);
            return NULL;
        }
    }
    fclose(file);
    return newBoard;
}

/**
 * this function receives a sudoku struct, and prints it's board
 * @param boardToPrint - a sudoku struct to print
 */
void printBoard(sudoku* boardToPrint)
{
    printf("%d\n", boardToPrint->size);
    for (int i = 0; i < boardToPrint->size; ++i)
    {
        for (int j = 0; j <boardToPrint->size ; ++j)
        {
            if (j == boardToPrint->size - 1)
            {
                printf("%d", boardToPrint->board[i][j]);
            }
            else
            {
                printf("%d ", boardToPrint->board[i][j]);
            }
        }
            printf("\n");
    }
}

int main(int argc, char *argv[])
{
    if(argc != NUM_OF_ARGUMNETS)
    {
        return EXIT_FAILURE;
    }
    sudoku* sudokuBoard;
    sudoku *winner;
    int exitCode = EXIT_SUCCESS;

    //create board from file
    sudokuBoard = (createBoardFromFile(argv[FILE_NAME_IDX], &exitCode));
    if(sudokuBoard == NULL)
    {
        return exitCode;
    }

    //calculate desired value (maximum number of solved cells)
    unsigned int desiredResult = (unsigned int)(*sudokuBoard).size *
                                 (unsigned int)(*sudokuBoard).size;
    //solve the given sudoku
    winner = getBest(sudokuBoard, getSudokuOptions, numOfSolvedCells, freeBoard, copyBoard,
                     desiredResult);
    if(numOfSolvedCells(winner) != desiredResult)
    {
        printf(NO_SOLUTION_MSG);
        exitCode = NO_SOLUTION_EXIT;
    }
    else
    {
        printBoard(winner);
    }
    freeBoard(sudokuBoard);
    freeBoard(winner);
    return exitCode;
}