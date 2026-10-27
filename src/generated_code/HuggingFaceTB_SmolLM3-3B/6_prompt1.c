#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char* convert(char* s, int numRows) {
    if (numRows == 1 || numRows >= strlen(s)) {
        return strdup(s);
    }

    int len = strlen(s);
    int rows = numRows;
    int cols = (len + rows - 1) / rows;
    int* col = (int*)malloc(cols * sizeof(int));
    memset(col, 0, cols * sizeof(int));
    int* row = (int*)malloc(rows * sizeof(int));
    memset(row, 0, rows * sizeof(int));

    for (int i = 0; i < len; i++) {
        int r = i % rows;
        int c = i / rows;
        col[c] += 1;
        row[r] += 1;
    }

    char* result = (char*)malloc(len * sizeof(char));
    int idx = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < col[i]; j++) {
            result[idx++] = s[row[i] + j * (rows - 1)];
        }
    }

    free(col);
    free(row);
    return result;
}

int main() {
    char* s = "PAYPALISHIRING";
    int numRows = 3;
    char* result = convert(s, numRows);
    printf("%s\n", result);
    free(result);
    return 0;
}
