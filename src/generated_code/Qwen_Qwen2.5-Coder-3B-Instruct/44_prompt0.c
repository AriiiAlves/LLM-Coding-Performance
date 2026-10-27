#include <stdio.h>
#include <string.h>

bool isMatch(char *s, char *p) {
    int m = strlen(s);
    int n = strlen(p);
    bool dp[m + 1][n + 1];

    // Initialize the dp table
    for (int i = 0; i <= m; i++) {
        for (int j = 0; j <= n; j++) {
            dp[i][j] = false;
        }
    }

    // Base case: empty pattern matches empty string
    dp[0][0] = true;

    // Handle patterns with '*' at the beginning
    for (int j = 1; j <= n; j++) {
        if (p[j - 1] == '*') {
            dp[0][j] = dp[0][j - 1];
        }
    }

    // Fill the dp table
    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (p[j - 1] == s[i - 1] || p[j - 1] == '?') {
                dp[i][j] = dp[i - 1][j - 1];
            } else if (p[j - 1] == '*') {
                dp[i][j] = dp[i - 1][j] || dp[i][j - 1];
            }
        }
    }

    // The result is in the bottom-right corner of the dp table
    return dp[m][n];
}

int main() {
    char s[] = "aa";
    char p[] = "*";
    printf("%d\n", isMatch(s, p));
    return 0;
}
