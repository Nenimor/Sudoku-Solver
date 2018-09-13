Sudoku solver, implemented in C (C workshop)

Input: a text file which represents a Sudoku board.
Output: a valid solution of the given Sudoku (if exists).

High-level of the algorithm:
The algorithm translates the given text file into a 2-dimensional integer matrix. Then in each step creates all possible valid assignments to a single unsolved cell, creating a tree of Sudoku boards, while the root is the given input, each node is a partially solved Sudoku board, and all leaves are fully solved boards or partially solved boards without further solving possibilities. 
At last, the algorithm runs over the whole tree, using DFS, finding a fully solved board (if exists), and returns it. The algorithm manages and frees all memory used while solving the Sudoku.
