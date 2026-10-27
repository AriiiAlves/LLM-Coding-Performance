#include <stdio.h>

bool isMatch(char *s, char *p) {
    int m = strlen(s), n = strlen(p);
    bool dp[m + 1][n + 1];
    memset(dp, false, sizeof(dp));
    
    // Base case: empty pattern matches empty string
    dp[0][0] = true;
    
    // Handle patterns like "a*" which can match zero characters
    for (int j = 2; j <= n; j += 2) {
        if (p[j - 1] == '*') {
            dp[0][j] = dp[0][j - 2];
        }
    }
    
    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (s[i - 1] == p[j - 1] || p[j - 1] == '.') {
                dp[i][j] = dp[i - 1][j - 1];
            } else if (p[j - 1] == '*') {
                dp[i][j] = dp[i][j - 2] || (dp[i - 1][j] && (s[i - 1] == p[j - 2] || p[j - 2] == '.'));
            }
        }
    }
    
    return dp[m][n];
}

int main() {
    printf("%d\n", isMatch("aa", "a"));  // Output: 0
    printf("%d\n", isMatch("aa", "a*")); // Output: 1
    printf("%d\n", isMatch("ab", ".*")); // Output: 1
    return 0;
}
