#include <stdio.h>
#include <string.h>

char * longestPalindrome(char * s) {
    int len = strlen(s);
    if (len < 2) return s;

    int maxLen = 1;
    int begin = 0;
    int i, j;

    // table[i][j] indicates whether s[i..j] is a palindrome
    int **table = (int **)malloc(sizeof(int *) * len);
    for(i = 0; i < len; i++)
        table[i] = (int *)malloc(sizeof(int) * len);

    for(i = 0; i < len; i++) {
        table[i][i] = 1;
        for(j = i - 1; j >= 0; j--) {
            if(s[i] == s[j]) {
                if(i - j < 3)
                    table[j][i] = 1;
                else
                    table[j][i] = table[j + 1][i - 1];
            }
            if(table[j][i] && i - j + 1 > maxLen) {
                maxLen = i - j + 1;
                begin = j;
            }
        }
    }

    char *res = (char *)malloc(sizeof(char) * (maxLen + 1));
    for(i = begin; i < begin + maxLen; i++) {
        res[i - begin] = s[i];
    }
    res[maxLen] = '\0';

    for(i = 0; i < len; i++)
        free(table[i]);
    free(table);

    return res;
}

int main() {
    char s[] = "babad";
    printf("%s\n", longestPalindrome(s));
    return 0;
}
