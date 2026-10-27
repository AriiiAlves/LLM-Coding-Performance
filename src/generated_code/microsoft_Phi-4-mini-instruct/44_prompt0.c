#include <stdio.h>
#include <string.h>

int isMatch(char* s, char* p) {
    int sl = strlen(s);
    int pl = strlen(p);
    int dp[sl + 1][pl + 1];
    
    for (int i = 0; i <= sl; i++) {
        for (int j = 0; j <= pl; j++) {
            if (j == 0) {
                dp[i][j] = (i == 0);
            } else if (i == 0) {
                dp[i][j] = (j == 1 && p[j - 1] == '*');
            } else {
                if (p[j - 1] == '*') {
                    dp[i][j] = dp[i][j - 1] || dp[i - 1][j];
                } else {
                    dp[i][j] = (p[j - 1] == '?' || s[i - 1] == p[j - 1]) && dp[i - 1][j - 1];
                }
            }
        }
    }
    return dp[sl][pl];
}

int main() {
    char s[] = "aa";
    char p[] = "*";
    printf("%d\n", isMatch(s, p));
    return 0;
}
