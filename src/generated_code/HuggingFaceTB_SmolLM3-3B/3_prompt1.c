#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int lengthOfLongestSubstring(char *s) {
    int max_length = 0;
    int start = 0;
    int char_set[128] = {0}; // ASCII characters

    for (int i = 0; s[i] != '\0'; i++) {
        if (char_set[(unsigned char)s[i]] > 0) {
            start = max(start, char_set[(unsigned char)s[i]]);
        }
        char_set[(unsigned char)s[i]] = i + 1;
        max_length = (max_length < i - start + 1) ? (i - start + 1) : max_length;
    }

    return max_length;
}

int main() {
    char s[] = "abcabcbb";
    int result = lengthOfLongestSubstring(s);
    printf("%d\n", result);
    return 0;
}
