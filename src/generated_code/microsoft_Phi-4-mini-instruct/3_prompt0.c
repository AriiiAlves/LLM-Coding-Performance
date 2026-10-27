#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int lengthOfLongestSubstring(char* s) {
    int maxLength = 0;
    int start = 0;
    int charIndex[256] = {0}; // Assuming ASCII character set

    for (int i = 0; s[i] != '\0'; i++) {
        if (charIndex[s[i]] > start) {
            start = charIndex[s[i]] + 1;
        }
        charIndex[s[i]] = i;
        maxLength = (i - start + 1 > maxLength) ? (i - start + 1) : maxLength;
    }
    return maxLength;
}

int main() {
    char s[] = "abcabcbb";
    printf("%d\n", lengthOfLongestSubstring(s));
    return 0;
}
