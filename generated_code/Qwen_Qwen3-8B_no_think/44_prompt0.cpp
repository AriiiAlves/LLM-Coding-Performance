#include <string>
using namespace std;

bool isMatch(string s, string p) {
    int sLen = s.length(), pLen = p.length();
    int sIdx = 0, pIdx = 0;
    int starIdx = -1, sStar = -1;

    while (sIdx < sLen) {
        if (pIdx < pLen && (p[pIdx] == '?' || p[pIdx] == s[sIdx])) {
            sIdx++;
            pIdx++;
        } else if (pIdx < pLen && p[pIdx] == '*') {
            starIdx = pIdx++;
            sStar = sIdx;
        } else {
            if (starIdx == -1) return false;
            pIdx = starIdx + 1;
            sIdx = sStar + 1;
        }
    }

    while (pIdx < pLen && p[pIdx] == '*') pIdx++;
    return pIdx == pLen;
}
