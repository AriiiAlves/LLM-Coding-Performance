// Reference: binary search on the partition of the smaller array, O(log(min(m, n)))
double findMedianSortedArrays(vector<int>& a, vector<int>& b) {
    if (a.size() > b.size()) return findMedianSortedArrays(b, a);
    int m = a.size(), n = b.size(), half = (m + n + 1) / 2, lo = 0, hi = m;
    while (lo <= hi) {
        int i = (lo + hi) / 2, j = half - i;
        int aL = i ? a[i - 1] : INT_MIN, aR = i < m ? a[i] : INT_MAX;
        int bL = j ? b[j - 1] : INT_MIN, bR = j < n ? b[j] : INT_MAX;
        if (aL > bR) hi = i - 1;
        else if (bL > aR) lo = i + 1;
        else {
            int L = max(aL, bL);
            if ((m + n) & 1) return L;
            return ((long long)L + min(aR, bR)) / 2.0;
        }
    }
    return 0.0;
}
