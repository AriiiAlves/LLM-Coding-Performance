#include <stdio.h>
#include <string.h>
#include <limits.h>

int lengthOfLongestSubstring(char* s) {
    int n = strlen(s);
    int maxLength = 0;
    int start = 0;
    int charIndex[256] = {0};

    for (int i = 0; i < n; i++) {
        if (charIndex[s[i]] > start) {
            start = charIndex[s[i]] + 1;
        }
        charIndex[s[i]] = i;
        maxLength = (i - start + 1 > maxLength) ? (i - start + 1) : maxLength;
    }
    return maxLength;
}

int main() {
    char s[] = "pwwkew";
    printf("%d\n", lengthOfLongestSubstring(s));
    return 0;
}
