#include <stdbool.h>
#include <string.h>
#include <stdio.h>

bool isMatch(char * s, char * p) {
    int m = strlen(s), n = strlen(p);
    bool dp[m + 1][n + 1];

    dp[0][0] = true;
    for (int i = 1; i <= n; i++) {
        if (p[i - 1] == '*' && dp[0][i - 1]) {
            dp[0][i] = true;
        }
    }

    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (p[j - 1] == '*') {
                dp[i][j] = dp[i - 1][j] || dp[i][j - 1];
            } else if (p[j - 1] == '?' || s[i - 1] == p[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1];
            }
        }
    }

    return dp[m][n];
}

int main() {
    char s[100], p[100];
    scanf("%s %s", s, p);
    if (isMatch(s, p)) {
        printf("true\n");
    } else {
        printf("false\n");
    }
    return 0;
}
