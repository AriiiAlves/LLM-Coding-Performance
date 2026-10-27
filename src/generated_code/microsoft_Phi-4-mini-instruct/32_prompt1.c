#include <stdio.h>
#include <string.h>

int longestValidParentheses(const char* s) {
    int maxLen = 0, left = 0, right = 0;
    int n = strlen(s);
    for (int i = 0; i < n; i++) {
        if (s[i] == '(') {
            left++;
        } else {
            right++;
        }
        if (left == right) {
            maxLen = right + left;
        } else if (right > left) {
            left = right = 0;
        }
    }
    return maxLen;
}

int main() {
    char s[] = ")()())";
    printf("%d\n", longestValidParentheses(s));
    return 0;
}
