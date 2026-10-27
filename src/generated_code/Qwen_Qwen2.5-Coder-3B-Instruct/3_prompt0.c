#include <stdio.h>
#include <string.h>

int lengthOfLongestSubstring(char *s) {
    int n = strlen(s);
    if (n == 0) return 0;

    int left = 0;
    int right = 0;
    int maxLength = 0;
    int charIndex[128] = {0};

    while (right < n) {
        charIndex[s[right]]++;
        while (charIndex[s[right]] > 1) {
            charIndex[s[left]]--;
            left++;
        }
        maxLength = fmax(maxLength, right - left + 1);
        right++;
    }

    return maxLength;
}

int main() {
    char s[] = "abcabcbb";
    printf("%d\n", lengthOfLongestSubstring(s));
    return 0;
}
