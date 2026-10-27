#include <stdio.h>
#include <string.h>

// Function to check if the string matches the pattern
int isMatch(char *s, char *p) {
    int s_len = strlen(s);
    int p_len = strlen(p);
    int dp[s_len + 1][p_len + 1] = {0}; // Initialize DP table

    // Base cases
    dp[0][0] = 1; // Empty string matches empty pattern
    for (int i = 0; i <= p_len; i++) {
        if (i == 0) continue;
        if (p[i - 1] == '*') {
            dp[0][i] = dp[0][i - 2];
        }
    }

    // Fill DP table
    for (int i = 0; i <= s_len; i++) {
        for (int j = 0; j <= p_len; j++) {
            if (p[j] == '*') {
                // Case 1: '*' matches zero occurrences of preceding character
                dp[i][j] = dp[i][j - 2];
                // Case 2: '*' matches one or more occurrences of preceding character
                if (j > 0 && (p[j - 1] == '.' || p[j - 1] == s[i - 1])) {
                    dp[i][j] |= dp[i - 1][j];
                }
            } else {
                // Case 1: current character matches
                if (j > 0 && (p[j - 1] == '.' || p[j - 1] == s[i - 1])) {
                    dp[i][j] = dp[i - 1][j - 1];
                }
            }
        }
    }

    return dp[s_len][p_len];
}

int main() {
    char s[] = "aa";
    char p[] = "a*";
    if (isMatch(s, p)) {
        printf("true\n");
    } else {
        printf("false\n");
    }
    return 0;
}
