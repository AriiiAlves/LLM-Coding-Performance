// Reference: bitmasks + minimum-remaining-values backtracking
static bool refSolve(vector<vector<char>>& b, int* row, int* col, int* box) {
    int br = -1, bc = -1, bmask = 0, bn = 10;
    for (int r = 0; r < 9; r++)
        for (int c = 0; c < 9; c++)
            if (b[r][c] == '.') {
                int mask = 0x1FF & ~(row[r] | col[c] | box[r / 3 * 3 + c / 3]);
                int n = __builtin_popcount(mask);
                if (n < bn) { bn = n; br = r; bc = c; bmask = mask; }
            }
    if (br < 0) return true;
    while (bmask) {
        int bit = bmask & -bmask; bmask ^= bit;
        int k = br / 3 * 3 + bc / 3;
        b[br][bc] = '1' + __builtin_ctz(bit);
        row[br] |= bit; col[bc] |= bit; box[k] |= bit;
        if (refSolve(b, row, col, box)) return true;
        row[br] ^= bit; col[bc] ^= bit; box[k] ^= bit;
    }
    b[br][bc] = '.';
    return false;
}

void solveSudoku(vector<vector<char>>& board) {
    int row[9] = {}, col[9] = {}, box[9] = {};
    for (int r = 0; r < 9; r++)
        for (int c = 0; c < 9; c++)
            if (board[r][c] != '.') {
                int bit = 1 << (board[r][c] - '1');
                row[r] |= bit; col[c] |= bit; box[r / 3 * 3 + c / 3] |= bit;
            }
    refSolve(board, row, col, box);
}
