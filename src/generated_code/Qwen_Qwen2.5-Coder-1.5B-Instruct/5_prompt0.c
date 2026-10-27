#include <stdio.h>
#include <string.h>

char* longestPalindrome(char* s) {
    int n = strlen(s);
    if (n == 0) return "";
    
    // Initialize a table to store whether substrings are palindromes
    bool dp[n][n];
    memset(dp, false, sizeof(dp));
    
    // All single characters are palindromes
    for (int i = 0; i < n; ++i) dp[i][i] = true;
    
    // Check for substrings of length 2
    for (int i = 0; i < n - 1; ++i) {
        if (s[i] == s[i + 1]) dp[i][i + 1] = true;
    }
    
    // Check for substrings of length greater than 2
    for (int len = 3; len <= n; ++len) {
        for (int i = 0; i <= n - len; ++i) {
            int j = i + len - 1;
            if (s[i] == s[j] && dp[i + 1][j - 1]) {
                dp[i][j] = true;
            }
        }
    }
    
    // Find the longest palindrome
    int start = 0, end = 0;
    for (int i = 0; i < n; ++i) {
        for (int j = i; j < n; ++j) {
            if (dp[i][j] && j - i > end - start) {
                start = i;
                end = j;
            }
        }
    }
    
    char* result = (char*)malloc((end - start + 2) * sizeof(char));
    strncpy(result, s + start, end - start + 1);
    result[end - start + 1] = '\0';
    
    return result;
}

int main() {
    char input[] = "babad";
    printf("%s\n", longestPalindrome(input)); // Output: "bab"
    
    input = "cbbd";
    printf("%s\n", longestPalindrome(input)); // Output: "bb"
    
    return 0;
}
