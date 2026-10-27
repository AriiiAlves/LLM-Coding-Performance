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
        result = realloc(result, (i + 1) * sizeof(char));
        if (row == 0) {
            row += step;
        } else {
            row -= step;
        }
        if (row >= numRows) {
            row = numRows - 1;
            step = -step;
        }
        strcat(result, &s[i]);
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
