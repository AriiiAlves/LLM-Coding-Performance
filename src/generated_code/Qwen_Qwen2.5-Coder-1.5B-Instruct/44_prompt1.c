#include <stdio.h>

bool isMatch(char *s, char *p) {
    int m = strlen(s);
    int n = strlen(p);

    // Create a DP table to store results of subproblems
    bool dp[m + 1][n + 1];

    // Initialize the DP table
    dp[0][0] = true; // Empty string matches empty pattern

    // Handle patterns starting with '*'
    for (int j = 1; j <= n; ++j) {
        if (p[j - 1] == '*') {
            dp[0][j] = dp[0][j - 1];
        }
    }

    // Fill the DP table
    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {
            if (p[j - 1] == '*') {
                dp[i][j] = dp[i][j - 1] || dp[i - 1][j];
            } else if (p[j - 1] == '?' || s[i - 1] == p[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1];
            }
        }
    }

    return dp[m][n];
}

int main() {
    printf("%d\n", isMatch("aa", "a")); // Output: 0
    printf("%d\n", isMatch("aa", "*")); // Output: 1
    printf("%d\n", isMatch("cb", "?a")); // Output: 0
    return 0;
}
