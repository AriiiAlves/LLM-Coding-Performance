#include <vector>
using namespace std;

bool is_valid(int row, int col, char num, vector<vector<char>>& board) {
    for (int i = 0; i < 9; i++) {
        if (board[row][i] == num) return false;
    }
    for (int i = 0; i < 9; i++) {
        if (board[i][col] == num) return false;
    }
    int start_row = row - row % 3;
    int start_col = col - col % 3;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[start_row + i][start_col + j] == num) return false;
        }
    }
    return true;
}

bool find_empty(int& row, int& col, vector<vector<char>>& board) {
    for (row = 0; row < 9; row++) {
        for (col = 0; col < 9; col++) {
            if (board[row][col] == '.') return true;
        }
    }
    return false;
}

bool backtrack(vector<vector<char>>& board) {
    int row, col;
    if (!find_empty(row, col, board)) return true;
    for (int num = 1; num <= 9; num++) {
        char num_char = '0' + num;
        if (is_valid(row, col, num_char, board)) {
            board[row][col] = num_char;
            if (backtrack(board)) return true;
            board[row][col] = '.';
        }
    }
    return false;
}

void solveSudoku(vector<vector<char>>& board) {
    backtrack(board);
}
