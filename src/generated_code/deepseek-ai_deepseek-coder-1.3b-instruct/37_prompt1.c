#include <stdio.h>
#include <stdlib.h>

void solveSudoku(char** board, int boardSize, int* boardColSize) {
    if (board == NULL || boardSize < 1 || boardColSize == NULL || *boardColSize < 1) {
        return;
    }

    int row = -1, col = -1;
    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < *boardColSize; j++) {
            if (board[i][j] == '.') {
                row = i;
                col = j;
                break;
            }
        }
        if (row != -1) {
            break;
        }
    }

    if (row == -1 && col == -1) {
        return;
    }

    for (int num = 1; num <= 9; num++) {
        if (isValid(board, row, col, num)) {
            board[row][col] = num + '0';
            if (solveSudoku(board, boardSize, boardColSize)) {
                return;
            }
            board[row][col] = '.';
        }
    }
}

bool isValid(char** board, int row, int col, int num) {
    for (int i = 0; i < 9; i++) {
        if (board[i][col] == num + '0') {
            return false;
        }
        if (board[row][i] == num + '0') {
            return false;
        }
    }

    int startRow = row - row % 3;
    int startCol = col - col % 3;
    for (int i = startRow; i < startRow + 3; i++) {
        for (int j = startCol; j < startCol + 3; j++) {
            if (board[i][j] == num + '0') {
                return false;
            }
        }
    }

    return true;
}

int main() {
    char* board[9] = {
        "53..7.",
        "6..195",
        ".98...6",
        "8...6..",
        "4.8...6",
        "...6...3",
        "...1...9",
        "...2...8",
        "...4...5"
    };

    int boardSize = sizeof(board) / sizeof(board[0]);
    int boardColSize = 9;

    solveSudoku((char**)board, boardSize, &boardColSize);

    for (int i = 0; i < boardSize; i++) {
        printf("%s\n", board[i]);
    }

    return 0;
}
