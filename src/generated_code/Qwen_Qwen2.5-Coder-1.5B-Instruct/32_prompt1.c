int longestValidParentheses(char* s) {
    int n = strlen(s);
    if(n == 0)
        return 0;
    
    int dp[n];
    memset(dp, 0, sizeof(dp));
    int maxLen = 0;
    
    for(int i = 1; i < n; i++){
        if(s[i] == ')' && s[i - 1] == '('){
            dp[i] = dp[i - 2] + 2;
        } else if(i > 1 && s[i] == ')' && s[i - 1] == ')' && s[i - dp[i - 1] - 1] == '('){
            dp[i] = dp[i - 1] + dp[i - dp[i - 1] - 2] + 2;
        }
        
        maxLen = fmax(maxLen, dp[i]);
    }
    
    return maxLen;
}
