#include <stdio.h>
#include <string.h>

int longestValidParentheses(char *s) {
    int n = strlen(s);
    if (n == 0) return 0;

    int max_len = 0;
    int stack[n];
    int top = -1;

    for (int i = 0; i < n; i++) {
        if (s[i] == '(') {
            stack[++top] = i;
        } else if (s[i] == ')' && top != -1 && s[stack[top]] == '(') {
            top--;
            max_len = fmax(max_len, i - (top == -1 ? -1 : stack[top]) + 1);
        }
    }

    return max_len;
}

int main() {
    char s[] = ")()())";
    printf("%d\n", longestValidParentheses(s));
    return 0;
}
