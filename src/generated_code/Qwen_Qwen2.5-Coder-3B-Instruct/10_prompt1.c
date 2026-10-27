#include <stdio.h>
#include <stdbool.h>

bool isMatch(const char *s, const char *p) {
    int m = strlen(s);
    int n = strlen(p);

    // Create a DP table to store results of subproblems
    bool dp[m + 1][n + 1];
    memset(dp, false, sizeof(dp));

    // Base case: empty pattern matches empty string
    dp[0][0] = true;

    // Handle patterns with '*' at the beginning
    for (int j = 1; j <= n; j++) {
        if (p[j - 1] == '*') {
            dp[0][j] = dp[0][j - 2];
        }
    }

    // Fill the DP table
    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (p[j - 1] == s[i - 1] || p[j - 1] == '.') {
                dp[i][j] = dp[i - 1][j - 1];
            } else if (p[j - 1] == '*') {
                dp[i][j] = dp[i][j - 2] || (dp[i - 1][j] && (s[i - 1] == p[j - 2] || p[j - 2] == '.'));
            }
        }
    }

    return dp[m][n];
}

int main() {
    const char *s = "aa";
    const char *p = "a*";
    printf("%d\n", isMatch(s, p)); // Output: 1 (true)
    return 0;
}
