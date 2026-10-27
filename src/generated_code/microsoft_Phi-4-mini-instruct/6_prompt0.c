#include <stdio.h>
#include <string.h>

void convert(char *s, int numRows) {
    if (numRows == 1) {
        printf("%s", s);
        return;
    }

    int len = strlen(s);
    int row = 0;
    int step = 1;
    char *result = (char *)malloc((len + 1) * sizeof(char));
    result[0] = '\0';

    for (int i = 0; i < len; i++) {
        result[row] = s[i];
        row += step;

        if (row >= numRows - 1) {
            row = numRows - 2;
            step = -1;
        } else if (row < 0) {
            row = 1;
            step = 1;
        }
    }

    printf("%s", result);
    free(result);
}

int main() {
    char s[] = "PAYPALISHIRING";
    int numRows = 3;
    convert(s, numRows);
    return 0;
}
