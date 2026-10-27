#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char* longestPalindrome(char* s) {
    int n = strlen(s);
    if (n == 0) return "";
    
    char* longest = malloc((n + 1) * sizeof(char));
    int maxLength = 0;
    int start = 0;
    
    for (int i = 0; i < n; i++) {
        int len1 = expandAroundCenter(s, i, i);
        int len2 = expandAroundCenter(s, i, i + 1);
        int len = len1 > len2 ? len1 : len2;
        if (len > maxLength) {
            maxLength = len;
            start = i - (len - 1) / 2;
        }
    }
    
    strncpy(longest, s + start, maxLength);
    longest[maxLength] = '\0';
    return longest;
}

int expandAroundCenter(char* s, int left, int right) {
    while (left >= 0 && right < strlen(s) && s[left] == s[right]) {
        left--;
        right++;
    }
    return right - left - 1;
}

int main() {
    char* s = "babad";
    char* result = longestPalindrome(s);
    printf("%s\n", result);
    return 0;
}
