#include <string>
using namespace std;

bool isMatch(string s, string p) {
    int sLen = s.length(), pLen = p.length();
    bool dp[pLen + 1][sLen + 1] = {false};

    dp[0][0] = true;

    for (int i = 1; i <= pLen; ++i) {
        if (p[i - 1] == '*') {
            dp[i][0] = dp[i - 1][0];
        }
    }

    for (int i = 1; i <= pLen; ++i) {
        for (int j = 1; j <= sLen; ++j) {
            if (p[i - 1] == '.' || p[i - 1] == s[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1];
            } else if (p[i - 1] == '*') {
                dp[i][j] = dp[i - 1][j] || dp[i][j - 1];
            }
        }
    }

    return dp[pLen][sLen];
}
