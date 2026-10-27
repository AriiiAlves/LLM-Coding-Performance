#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 9

typedef struct {
    char board[N][N];
} Sudoku;

void print_board(Sudoku *s) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%c ", s->board[i][j]);
        }
        printf("\n");
    }
}

int is_valid(Sudoku *s, int row, int col, char num) {
    // Check row
    for (int i = 0; i < N; i++) {
        if (s->board[row][i] == num) return 0;
    }
    // Check column
    for (int i = 0; i < N; i++) {
        if (s->board[i][col] == num) return 0;
    }
    // Check sub-box
    int sub_row = row / 3 * 3;
    int sub_col = col / 3 * 3;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (s->board[sub_row + i][sub_col + j] == num) return 0;
        }
    }
    return 1;
}

int solve(Sudoku *s) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (s->board[i][j] == '.') {
                for (char num = '1'; num <= '9'; num++) {
                    if (is_valid(s, i, j, num)) {
                        s->board[i][j] = num;
                        if (solve(s)) return 1;
                        s->board[i][j] = '.';
                    }
                }
                return 0;
            }
        }
    }
    return 1;
}

int main() {
    Sudoku s;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            scanf("%c", &s.board[i][j]);
        }
    }
    solve(&s);
    print_board(&s);
    return 0;
}
