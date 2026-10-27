class Solution {
public:
    bool isMatch(string s, string p) {
        int m = s.size(), n = p.size();
        int i = 0, j = 0, starIdx = -1, match = 0;
        while (i < m) {
            if (j < n && (p[j] == s[i] || p[j] == '?')) {
                i++;
                j++;
            } else if (j < n && p[j] == '*') {
                starIdx = j;
                match = i;
                j++;
            } else if (starIdx != -1) {
                j = starIdx + 1;
                match++;
                i = match;
            } else {
                return false;
            }
        }
        while (j < n && p[j] == '*') {
            j++;
        }
        return j == n;
    }
};