#include <stdio.h>
#include <string.h>

// Function to check if the pattern matches the string
int isMatch(char *s, char *p) {
    int s_len = strlen(s);
    int p_len = strlen(p);
    int dp[s_len + 1][p_len + 1] = {0}; // Initialize DP table

    // Base cases
    dp[0][0] = 1; // Empty string matches empty pattern
    for (int i = 0; i <= p_len; i++) {
        if (i == 0) continue;
        if (p[i - 1] == '*') {
            dp[0][i] = dp[0][i - 1];
        } else {
            break;
        }
    }

    // Fill DP table
    for (int i = 0; i <= s_len; i++) {
        for (int j = 0; j <= p_len; j++) {
            if (i > 0 && j > 0) {
                if (s[i - 1] == p[j - 1] || p[j - 1] == '?') {
                    dp[i][j] = dp[i - 1][j - 1];
                } else if (p[j - 1] == '*') {
                    dp[i][j] = dp[i][j - 1] || dp[i - 1][j];
                } else {
                    dp[i][j] = 0;
                }
            } else {
                dp[i][j] = 0;
            }
        }
    }

    return dp[s_len][p_len];
}

int main() {
    char s[2001], p[2001];
    scanf("%s", s);
    scanf("%s", p);
    printf("%d\n", isMatch(s, p));
    return 0;
}
