#include <stdio.h>
#include <string.h>

bool isMatch(const char *s, const char *p) {
    int m = strlen(s);
    int n = strlen(p);

    // Create a DP table
    bool dp[m + 1][n + 1];
    memset(dp, false, sizeof(dp));

    // Base case: empty pattern matches empty string
    dp[0][0] = true;

    // Handle patterns with '*' at the beginning
    for (int j = 1; j <= n; j++) {
        if (p[j - 1] == '*') {
            dp[0][j] = dp[0][j - 1];
        }
    }

    // Fill the DP table
    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (p[j - 1] == s[i - 1] || p[j - 1] == '?') {
                dp[i][j] = dp[i - 1][j - 1];
            } else if (p[j - 1] == '*') {
                dp[i][j] = dp[i - 1][j] || dp[i][j - 1];
            }
        }
    }

    // The result is in the bottom-right cell of the DP table
    return dp[m][n];
}

int main() {
    char s[] = "aa";
    char p[] = "a";
    printf("%d\n", isMatch(s, p)); // Output: 0

    char s2[] = "aa";
    char p2[] = "*";
    printf("%d\n", isMatch(s2, p2)); // Output: 1

    char s3[] = "cb";
    char p3[] = "?a";
    printf("%d\n", isMatch(s3, p3)); // Output: 0

    return 0;
}
