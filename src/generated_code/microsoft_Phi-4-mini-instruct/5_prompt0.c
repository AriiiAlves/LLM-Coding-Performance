#include <stdio.h>
#include <string.h>

void longestPalindromeSubstring(char* s) {
    int n = strlen(s);
    if (n < 2) return;

    int start = 0, end = 0;
    for (int i = 0; i < n; i++) {
        int len1 = expandAroundCenter(s, i, i);
        int len2 = expandAroundCenter(s, i, i + 1);
        int len = len1 > len2 ? len1 : len2;
        if (len > end - start) {
            start = i - (len - 1) / 2;
            end = i + len / 2;
        }
    }
    printf("%s\n", s + start);
}

int expandAroundCenter(char* s, int left, int right) {
    while (left >= 0 && right < strlen(s) && s[left] == s[right]) {
        left--;
        right++;
    }
    return right - left - 1;
}

int main() {
    char s[] = "babad";
    longestPalindromeSubstring(s);
    return 0;
}
