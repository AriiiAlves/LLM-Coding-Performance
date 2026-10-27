#include <stdio.h>
#include <string.h>

int isMatch(char* s, char* p) {
    int sl = strlen(s);
    int pl = strlen(p);
    int dp[sl + 1][pl + 1];
    
    for (int i = 0; i <= sl; i++) {
        for (int j = 0; j <= pl; j++) {
            dp[i][j] = 0;
        }
    }
    
    dp[0][0] = 1;
    
    for (int j = 1; j <= pl; j++) {
        if (p[j - 1] == '*') {
            dp[0][j] = dp[0][j - 1];
        }
    }
    
    for (int i = 1; i <= sl; i++) {
        for (int j = 1; j <= pl; j++) {
            if (p[j - 1] == s[i - 1] || p[j - 1] == '?') {
                dp[i][j] = dp[i - 1][j - 1];
            } else if (p[j - 1] == '*') {
                dp[i][j] = dp[i][j - 1] || dp[i - 1][j];
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
