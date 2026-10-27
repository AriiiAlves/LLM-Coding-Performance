class Solution {
public:
    vector<int> findSubstring(string s, vector<string>& words) {
        vector<int> res;
        if (s.empty() || words.empty()) return res;
        int wl = words[0].size();
        int nw = words.size();
        int total = wl * nw;
        if (s.size() < total) return res;
        unordered_map<string, int> need;
        for (auto& w : words) need[w]++;
        for (int offset = 0; offset < wl; offset++) {
            int left = offset, count = 0;
            unordered_map<string, int> window;
            for (int right = offset; right + wl <= (int)s.size(); right += wl) {
                string w = s.substr(right, wl);
                if (need.count(w)) {
                    window[w]++;
                    count++;
                    while (window[w] > need[w]) {
                        string lw = s.substr(left, wl);
                        window[lw]--;
                        count--;
                        left += wl;
                    }
                    if (count == nw) {
                        res.push_back(left);
                        string lw = s.substr(left, wl);
                        window[lw]--;
                        count--;
                        left += wl;
                    }
                } else {
                    window.clear();
                    count = 0;
                    left = right + wl;
                }
            }
        }
        return res;
    }
};