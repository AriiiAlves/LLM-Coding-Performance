#include <stdio.h>
#include <string.h>

char* convert(char* s, int numRows) {
    int len = strlen(s);
    int cycleLen = 2 * numRows - 2;
    char* res = malloc(len + 1);
    int resIdx = 0;

    for(int i = 0; i < numRows; i++) {
        if(i == 0 || i == numRows - 1) {
            for(int j = i; j < len; j += cycleLen) {
                res[resIdx++] = s[j];
            }
        } else {
            int j = i;
            while(j < len) {
                res[resIdx++] = s[j];
                int k = cycleLen - (j % cycleLen) - 2;
                if(k < numRows) {
                    res[resIdx++] = s[k];
                }
                j += cycleLen;
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
