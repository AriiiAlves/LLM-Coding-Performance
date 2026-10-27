#include <stdio.h>
#include <string.h>
#include <stdbool.h>

int lengthOfLongestSubstring(char* s) {
    int max_length = 0;
    int left = 0;
    int char_set[128] = {0}; // ASCII characters

    for (int right = 0; right < strlen(s); right++) {
        while (char_set[s[right]] != 0) {
            char_set[s[left]] = 0;
            left++;
        }
        char_set[s[right]] = 1;
        max_length = (max_length < right - left + 1) ? (right - left + 1) : max_length;
    }

    return max_length;
}

int main() {
    char s[] = "abcabcbb";
    printf("%d\n", lengthOfLongestSubstring(s));
    return 0;
}
