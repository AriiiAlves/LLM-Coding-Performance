#include <stdio.h>
#include <string.h>

char* convert(char* s, int numRows) {
    int n = strlen(s);
    int cycleLen = 2 * numRows - 2;
    char* res = malloc(n + 1);
    int resIdx = 0;

    for(int i = 0; i < numRows; i++) {
        if(i == 0 || i == numRows - 1) {
            for(int j = i; j < n; j += cycleLen) {
                res[resIdx++] = s[j];
            }
        } else {
            int k = i;
            while(k < n) {
                res[resIdx++] = s[k];
                int l = 2 * (numRows - i - 1) - 2;
                if(k + l < n) {
                    res[resIdx++] = s[k + l];
                }
                k += cycleLen;
            }
        }
    }

    res[resIdx] = '\0';
    return res;
}

int main() {
    char* s = "PAYPALISHIRING";
    int numRows = 3;
    char* res = convert(s, numRows);
    printf("%s\n", res);
    free(res);
    return 0;
}
