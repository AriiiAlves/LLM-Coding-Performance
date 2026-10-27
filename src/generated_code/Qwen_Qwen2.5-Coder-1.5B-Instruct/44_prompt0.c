#include <stdio.h>

bool isMatch(char *s, char *p) {
    int m = strlen(s);
    int n = strlen(p);

    // Create a DP table to store results of subproblems
    bool dp[m + 1][n + 1];

    // Initialize the DP table
    for (int i = 0; i <= m; i++) {
        for (int j = 0; j <= n; j++) {
            if (j == 0) {
                dp[i][j] = (i == 0);
            } else if (i == 0) {
                dp[i][j] = (p[j - 1] == '*' && dp[i][j - 1]);
            } else {
                if (p[j - 1] == '?') {
                    dp[i][j] = dp[i - 1][j - 1];
                } else if (p[j - 1] == '*') {
                    dp[i][j] = dp[i - 1][j] || dp[i][j - 1];
                } else {
                    dp[i][j] = (s[i - 1] == p[j - 1]) && dp[i - 1][j - 1];
                }
            }
        }
    }

    return dp[m][n];
}

int main() {
    char s[] = "aa";
    char p[] = "a";
    printf("%d\n", isMatch(s, p)); // Output: 0

    s[] = "aa";
    p[] = "*";
    printf("%d\n", isMatch(s, p)); // Output: 1

    s[] = "cb";
    p[] = "?a";
    printf("%d\n", isMatch(s, p)); // Output: 0

    return 0;
}
