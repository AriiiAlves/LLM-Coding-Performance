#include <vector>
using namespace std;

bool isValid(vector<vector<char>>& board, int row, int col, char num) {
    for (int i = 0; i < 9; ++i) {
        if (board[row][i] == num || board[i][col] == num) {
            return false;
        }
    }
    int boxRowStart = row - row % 3;
    int boxColStart = col - col % 3;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (board[i + boxRowStart][j + boxColStart] == num) {
                return false;
            }
        }
    }
    return true;
}

bool solveSudokuHelper(vector<vector<char>>& board) {
    for (int row = 0; row < 9; ++row) {
        for (int col = 0; col < 9; ++col) {
            if (board[row][col] == '.') {
                for (char num = '1'; num <= '9'; ++num) {
                    if (isValid(board, row, col, num)) {
                        board[row][col] = num;
                        if (solveSudokuHelper(board)) {
                            return true;
                        }
                        board[row][col] = '.';
                    }
                }
                return false;
            }
        }
    }
    return true;
}

void solveSudoku(vector<vector<char>>& board) {
    solveSudokuHelper(board);
}
