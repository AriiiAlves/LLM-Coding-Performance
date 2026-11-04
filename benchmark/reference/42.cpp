// Reference: two pointers, O(n) time, O(1) memory
int trap(vector<int>& h) {
    int l = 0, r = (int)h.size() - 1, lm = 0, rm = 0;
    long long water = 0;
    while (l < r) {
        if (h[l] < h[r]) { lm = max(lm, h[l]); water += lm - h[l]; l++; }
        else { rm = max(rm, h[r]); water += rm - h[r]; r--; }
    }
    return (int)water;
}
