class Solution {
public:
    bool isMatch(string s, string p) {
        int i = 0;  // Index in s
        int j = 0;  // Index in p

        int star = -1;
        int match = 0;

        while (i < (int)s.size()) {

            // Exact character or '?'.
            if (
                j < (int)p.size() &&
                (p[j] == '?' || p[j] == s[i])
            ) {
                i++;
                j++;
            }

            // Remember the most recent '*'.
            else if (
                j < (int)p.size() &&
                p[j] == '*'
            ) {
                star = j;
                match = i;
                j++;
            }

            // Previous '*' consumes one more character.
            else if (star != -1) {
                j = star + 1;
                match++;
                i = match;
            }

            // No '*' available to fix the mismatch.
            else {
                return false;
            }
        }

        // Remaining '*' characters can represent "".
        while (
            j < (int)p.size() &&
            p[j] == '*'
        ) {
            j++;
        }

        return j == (int)p.size();
    }
};