class Solution {
public:
    bool isMatch(string s, string p) {
        int i = 0, j = 0;
        int starIdx = -1, match = 0;

        while (i < (int)s.size()) {
            if (j < (int)p.size() && (p[j] == '?' || p[j] == s[i])) {
                i++;
                j++;
            } else if (j < (int)p.size() && p[j] == '*') {
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

        while (j < (int)p.size() && p[j] == '*') j++;
        return j == (int)p.size();
    }
};