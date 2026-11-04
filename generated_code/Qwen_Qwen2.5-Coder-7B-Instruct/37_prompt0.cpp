#include <vector>
using namespace std;

bool isValid(vector<vector<char>>& board, int row, int col, char num) {
    for (int x = 0; x < 9; x++) {
        if (board[row][x] == num || board[x][col] == num ||
            board[3 * (row / 3) + x / 3][3 * (col / 3) + x % 3] == num) {
            return false;
        }
    }
    return true;
}

bool solveSudokuHelper(vector<vector<char>>& board, int row, int col) {
    if (row == 9 - 1 && col == 9) {
        return true;
    }

    if (col == 9) {
        row++;
        col = 0;
    }

    if (board[row][col] != '.') {
        return solveSudokuHelper(board, row, col + 1);
    }

    for (char num = '1'; num <= '9'; num++) {
        if (isValid(board, row, col, num)) {
            board[row][col] = num;
            if (solveSudokuHelper(board, row, col + 1)) {
                return true;
            }
            board[row][col] = '.';
        }
    }

    return false;
}

void solveSudoku(vector<vector<char>>& board) {
    solveSudokuHelper(board, 0, 0);
}
