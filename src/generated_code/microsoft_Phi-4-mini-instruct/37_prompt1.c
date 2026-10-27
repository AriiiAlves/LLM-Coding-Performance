#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int isSafe(int board[9][9], int row, int col, int num) {
    for (int x = 0; x < 9; x++) {
        if (board[row][x] == num || board[x][col] == num || 
            board[3 * (row / 3) + x / 3][3 * (col / 3) + x % 3] == num) {
            return 0;
        }
    }
    return 1;
}

int solveSudoku(int board[9][9]) {
    int row, col;
    int emptySpot = -1;
    for (row = 0; row < 9; row++) {
        for (col = 0; col < 9; col++) {
            if (board[row][col] == '.') {
                emptySpot = row * 9 + col;
                break;
            }
        }
        if (emptySpot != -1) break;
    }
    if (emptySpot == -1) return 1;

    for (int num = 1; num <= 9; num++) {
        if (isSafe(board, emptySpot / 9, emptySpot % 9, num)) {
            board[emptySpot / 9][emptySpot % 9] = num;
            if (solveSudoku(board)) return 1;
            board[emptySpot / 9][emptySpot % 9] = '.';
        }
    }
    return 0;
}

void printBoard(int board[9][9]) {
    for (int r = 0; r < 9; r++) {
        for (int d = 0; d < 9; d++) {
            printf("%d ", board[r][d]);
        }
        printf("\n");
    }
}

int main() {
    int board[9][9] = {
        {5, 3, '.', '.', 7, '.', '.', '.', '.'},
        {6, '.', '.', 1, 9, 5, '.', '.', '.'},
        {'.', 9, 8, '.', '.', '.', '.', 6, '.'},
        {8, '.', '.', '.', 6, '.', '.', '.', 3},
        {4, '.', '.', 8, '.', 3, '.', '.', 1},
        {7, '.', '.', '.', 2, '.', '.', '.', 6},
        {'.', 6, '.', '.', '.', '.', 2, 8, '.'},
        {'.', '.', '.', 4, 1, 9, '.', '.', 5},
        {'.', '.', '.', '.', 8, '.', '.', 7, 9}
    };

    if (solveSudoku(board)) {
        printBoard(board);
    } else {
        printf("No solution exists\n");
    }

    return 0;
}
