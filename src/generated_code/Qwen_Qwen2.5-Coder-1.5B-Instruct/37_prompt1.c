#include <stdio.h>
#include <stdlib.h>

void solveSudoku(char** board, int boardSize, int* boardColSize) {
    if (!board || !boardSize || !boardColSize) return;

    // Helper function to check if a number can be placed at a given position
    bool isValid(int i, int j, char num, char** board, int size) {
        for (int k = 0; k < size; k++) {
            if (board[i][k] == num || board[k][j] == num) return false;
            if (board[3 * (i / 3) + k / 3][3 * (j / 3) + k % 3] == num) return false;
        }
        return true;
    }

    // Recursive function to solve the Sudoku
    bool solve(char** board, int size) {
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                if (board[i][j] == '.') {
                    for (char num = '1'; num <= '9'; num++) {
                        if (isValid(i, j, num, board, size)) {
                            board[i][j] = num;
                            if (solve(board, size)) return true;
                            board[i][j] = '.';
                        }
                    }
                    return false;
                }
            }
        }
        return true;
    }

    solve(board, size);
}
