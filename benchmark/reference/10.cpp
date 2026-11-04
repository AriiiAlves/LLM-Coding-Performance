// Reference: bottom-up DP, O(|s| * |p|)
bool isMatch(string s, string p) {
    int m = s.size(), n = p.size();
    vector<vector<char>> dp(m + 1, vector<char>(n + 1, 0));
    dp[m][n] = 1;
    for (int i = m; i >= 0; --i)
        for (int j = n - 1; j >= 0; --j) {
            bool first = i < m && (p[j] == s[i] || p[j] == '.');
            if (j + 1 < n && p[j + 1] == '*') dp[i][j] = dp[i][j + 2] || (first && dp[i + 1][j]);
            else dp[i][j] = first && dp[i + 1][j + 1];
        }
    return dp[0][0];
}
