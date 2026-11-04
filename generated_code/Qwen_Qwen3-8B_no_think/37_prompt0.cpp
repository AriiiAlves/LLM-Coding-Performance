#include <vector>
#include <algorithm>
using namespace std;

void solveSudoku(vector<vector<char>>& board) {
    vector<vector<bool>> rowUsed(9, vector<bool>(9, false));
    vector<vector<bool>> colUsed(9, vector<bool>(9, false));
    vector<vector<vector<bool>>> boxUsed(9, vector<vector<bool>>(9, vector<bool>(9, false)));

    // Initialize used arrays
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            if (board[i][j] != '.') {
                int num = board[i][j] - '0' - 1;
                rowUsed[i][num] = true;
                colUsed[j][num] = true;
                int boxIndex = (i / 3) * 3 + (j / 3);
                boxUsed[boxIndex][num] = true;
            }
        }
    }

    function<bool(int, int)> backtrack = [&](int row) {
        if (row == 9) return true;

        int col = 0;
        while (col < 9 && board[row][col] != '.') ++col;

        if (col < 9) {
            return backtrack(row + 1);
        }

        for (int num = 0; num < 9; ++num) {
            if (!rowUsed[row][num] && !colUsed[col][num]) {
                int boxIndex = (row / 3) * 3 + (col / 3);
                if (!boxUsed[boxIndex][num]) {
                    board[row][col] = '0' + num + 1;
                    rowUsed[row][num] = true;
                    colUsed[col][num] = true;
                    boxUsed[boxIndex][num] = true;

                    if (backtrack(row + 1)) return true;

                    // Backtrack
                    board[row][col] = '.';
                    rowUsed[row][num] = false;
                    colUsed[col][num] = false;
                    boxUsed[boxIndex][num] = false;
                }
            }
        }
        return false;
    };

    backtrack(0);
}
