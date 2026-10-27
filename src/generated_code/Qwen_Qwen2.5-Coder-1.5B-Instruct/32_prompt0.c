int longestValidParentheses(char* s) {
    int n = strlen(s);
    if (n == 0) return 0;
    
    int dp[n];
    memset(dp, 0, sizeof(dp));
    int max_length = 0;
    
    for (int i = 1; i < n; ++i) {
        if (s[i] == ')' && i - dp[i - 1] > 0 && s[i - dp[i - 1] - 1] == '(') {
            dp[i] = dp[i - 1] + 2 + (i - dp[i - 1] - 2 >= 0 ? dp[i - dp[i - 1] - 2] : 0);
            max_length = fmax(max_length, dp[i]);
        }
    }
    
    return max_length;
}
