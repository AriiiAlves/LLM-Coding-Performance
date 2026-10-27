#include <stdio.h>
#include <string.h>

char* longestPalindrome(char* s) {
    int n = strlen(s);
    if (n == 0) return "";
    
    // Initialize a table to store whether substrings are palindromes
    bool dp[n][n];
    memset(dp, false, sizeof(dp));
    
    // All single characters are palindromes
    for (int i = 0; i < n; ++i) {
        dp[i][i] = true;
    }
    
    // Check for palindromes of length 2
    for (int i = 0; i < n - 1; ++i) {
        if (s[i] == s[i + 1]) {
            dp[i][i + 1] = true;
        }
    }
    
    // Check for palindromes of length greater than 2
    for (int len = 3; len <= n; ++len) {
        for (int i = 0; i <= n - len; ++i) {
            int j = i + len - 1;
            if (s[i] == s[j] && dp[i + 1][j - 1]) {
                dp[i][j] = true;
            }
        }
    }
    
    // Find the longest palindrome
    int start = 0, maxLen = 0;
    for (int i = 0; i < n; ++i) {
        for (int j = i; j < n; ++j) {
            if (dp[i][j] && j - i + 1 > maxLen) {
                start = i;
                maxLen = j - i + 1;
            }
        }
    }
    
    char* result = (char*)malloc(maxLen + 1);
    strncpy(result, s + start, maxLen);
    result[maxLen] = '\0';
    
    return result;
}

int main() {
    char s[] = "babad";
    printf("%s\n", longestPalindrome(s)); // Output: "bab"
    
    char t[] = "cbbd";
    printf("%s\n", longestPalindrome(t)); // Output: "bb"
    
    return 0;
}
