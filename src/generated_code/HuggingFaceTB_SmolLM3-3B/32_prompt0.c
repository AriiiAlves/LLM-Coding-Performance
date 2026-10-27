#include <stdio.h>
#include <string.h>

int longestValidParentheses(const char *s) {
    int max_length = 0;
    int stack = -1; // Initialize stack with -1 to handle base case
    int length = strlen(s);

    for (int i = 0; i < length; i++) {
        if (s[i] == '(') {
            stack = i;
        } else {
            if (stack == -1) {
                stack = i;
            } else {
                if (s[stack] == '(') {
                    stack--;
                } else {
                    stack = i;
                }
            }
        }
        if (stack >= 0) {
            max_length = (i - stack + 1) > max_length ? (i - stack + 1) : max_length;
        }
    }

    return max_length;
}

int main() {
    const char *s = "(()";
    printf("%d\n", longestValidParentheses(s));
    return 0;
}
