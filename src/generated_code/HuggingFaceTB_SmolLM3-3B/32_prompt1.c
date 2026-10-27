#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int longestValidParentheses(const char *s) {
    int n = strlen(s);
    int maxLen = 0;
    int *dp = (int *)malloc((n + 1) * sizeof(int));
    memset(dp, 0, (n + 1) * sizeof(int));

    for (int i = 1; i <= n; i++) {
        if (s[i - 1] == '(') {
            dp[i] = 0;
        } else {
            if (s[i - 1] == ')' && i - dp[i - 1] - 1 >= 0 && s[i - dp[i - 1] - 1] == '(') {
                dp[i] = dp[i - 1] + dp[i - dp[i - 1] - 2] + 2;
            }
        }
        maxLen = (maxLen > dp[i]) ? maxLen : dp[i];
    }

    free(dp);
    return maxLen;
}

int main() {
    const char *s = "(()";
    printf("%d\n", longestValidParentheses(s));
    return 0;
}
