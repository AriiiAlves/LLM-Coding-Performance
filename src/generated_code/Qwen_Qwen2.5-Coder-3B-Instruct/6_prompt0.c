#include <stdio.h>
#include <string.h>

char* convert(char* s, int numRows) {
    if (numRows == 1) return s; // If there's only one row, return the original string

    int len = strlen(s);
    char* result = (char*)malloc((len + 1) * sizeof(char));
    result[len] = '\0';

    int index = 0;
    int step = 2 * numRows - 2;

    for (int i = 0; i < numRows; i++) {
        for (int j = i; j < len; j += step) {
            result[index++] = s[j];
            if (i != 0 && i != numRows - 1 && j + step - 2 * i < len) {
                result[index++] = s[j + step - 2 * i];
            }
        }
    }

    return result;
}

int main() {
    char s[] = "PAYPALISHIRING";
    int numRows = 3;
    char* result = convert(s, numRows);
    printf("%s\n", result);
    free(result);
    return 0;
}
