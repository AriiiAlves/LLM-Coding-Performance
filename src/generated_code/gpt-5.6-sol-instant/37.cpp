class Solution {
private:
    bool rows[9][10] = {};
    bool cols[9][10] = {};
    bool boxes[9][10] = {};

    int getBox(int row, int col) {
        return (row / 3) * 3 + (col / 3);
    }

    bool solve(vector<vector<char>>& board, int pos) {
        if (pos == 81)
            return true;

        int row = pos / 9;
        int col = pos % 9;

        if (board[row][col] != '.')
            return solve(board, pos + 1);

        int box = getBox(row, col);

        for (int num = 1; num <= 9; ++num) {
            if (rows[row][num] ||
                cols[col][num] ||
                boxes[box][num]) {
                continue;
            }

            board[row][col] = '0' + num;

            rows[row][num] = true;
            cols[col][num] = true;
            boxes[box][num] = true;

            if (solve(board, pos + 1))
                return true;

            // Backtrack.
            board[row][col] = '.';

            rows[row][num] = false;
            cols[col][num] = false;
            boxes[box][num] = false;
        }

        return false;
    }

public:
    void solveSudoku(vector<vector<char>>& board) {
        for (int row = 0; row < 9; ++row) {
            for (int col = 0; col < 9; ++col) {
                if (board[row][col] != '.') {
                    int num = board[row][col] - '0';
                    int box = getBox(row, col);

                    rows[row][num] = true;
                    cols[col][num] = true;
                    boxes[box][num] = true;
                }
            }
        }

        solve(board, 0);
    }
};