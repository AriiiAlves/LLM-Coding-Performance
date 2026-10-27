#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char* longestPalindromeSubstring(char* s) {
    int n = strlen(s);
    if (n < 1) return "";
    char* longest = s;
    for (int i = 0; i < n; i++) {
        for (int j = i; j < n; j++) {
            int len = j - i + 1;
            if (len > 1 && s[i] == s[j] && (len == 2 || longestPalindromeSubstring(s + i, s + j - 1) == s + i + j - i - 1)) {
                if (len > strlen(longest)) {
                    longest = (char*)malloc((len + 1) * sizeof(char));
                    strncpy(longest, s + i, len);
                    longest[len] = '\0';
                }
            }
        }
    }
    return longest;
}

int main() {
    char s[] = "babad";
    char* result = longestPalindromeSubstring(s);
    printf("%s\n", result);
    free(result);
    return 0;
}
