#include <stdio.h>

void solveSudoku(char** board, int boardSize) {
    for (int i = 0; i < boardSize; ++i) {
        for (int j = 0; j < boardSize; ++j) {
            if (board[i][j] == '.') {
                for (char num = '1'; num <= '9'; ++num) {
                    if (isValid(board, i, j, num)) {
                        board[i][j] = num;
                        if (solveSudoku(board, boardSize)) {
                            return true;
                        }
                        board[i][j] = '.';
                    }
                }
                return false;
            }
        }
    }
    return true;
}

bool isValid(char** board, int row, int col, char num) {
    for (int i = 0; i < 9; ++i) {
        if (board[row][i] == num || board[i][col] == num || board[3 * (row / 3) + i / 3][3 * (col / 3) + i % 3] == num) {
            return false;
        }
    }
    return true;
}

int main() {
    char board[9][9] = {
        {'5','3','.','.','7','.','.','.'},
        {'6','.','.','1','9','5','.','.'},
        {'.','9','8','.','.','.','.','6','.'},
        {'8','.','.','.','6','.','.','.','3'},
        {'4','.','.','8','.','3','.','.1'},
        {'7','.','.','.2','4','.','.','6'},
        {'.','6','.','.','.','.','2','8','.'},
        {'.','.','.','4','1','9','.','5'},
        {'.','.','.','.8','.','.','.7','9'}
    };
    solveSudoku(board, 9);
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            printf("%c ", board[i][j]);
        }
        printf("\n");
    }
    return 0;
}
