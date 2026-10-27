#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 9

int is_valid(char board[SIZE][SIZE], int row, int col, int num) {
    for (int x = 0; x < SIZE; x++)
        if (board[x][col] == num)
            return 0;
    for (int x = 0; x < SIZE; x++)
        if (board[row][x] == num)
            return 0;
    int start_row = row - row % 3, start_col = col - col % 3;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (board[i + start_row][j + start_col] == num)
                return 0;
    return 1;
}

int solve_sudoku(char board[SIZE][SIZE], int row, int col) {
    if (row == SIZE - 1 && col == SIZE)
        return 1;
    if (col == SIZE) {
        row++;
        col = 0;
    }
    if (board[row][col] != '.')
        return solve_sudoku(board, row, col + 1);
    for (int num = 1; num <= SIZE; num++) {
        if (is_valid(board, row, col, num)) {
            board[row][col] = num + '0';
            if (solve_sudoku(board, row, col + 1))
                return 1;
            board[row][col] = '.';
        }
    }
    return 0;
}

int main() {
    char board[SIZE][SIZE] = {
        {"5","3",".",".","7",".",".",".","."},
        {"6",".",".","1","9","5",".",".","."},
        {".","9","8",".",".",".",".","6","."},
        {"8",".",".",".","6",".",".",".","3"},
        {"4",".",".","8",".","3",".",".","1"},
        {"7",".",".",".","2",".",".",".","6"},
        {".","6",".",".",".",".","2","8","."},
        {".",".",".","4","1","9",".",".","5"},
        {".",".",".",".","8",".",".","7","9"}
    };
    if (solve_sudoku(board, 0, 0)) {
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++)
                printf("%c ", board[i][j]);
            printf("\n");
        }
    } else
        printf("No solution exists.\n");
    return 0;
}
