// Reference: sliding window per offset over word ids, O(|s| * wordLen)
vector<int> findSubstring(string s, vector<string>& words) {
    vector<int> res;
    if (words.empty()) return res;
    int L = words[0].size(), K = words.size(), n = s.size();
    if ((long long)L * K > n) return res;
    unordered_map<string, int> id;
    vector<int> need;
    for (auto& w : words) {
        auto it = id.find(w);
        if (it == id.end()) { id.emplace(w, (int)need.size()); need.push_back(1); }
        else need[it->second]++;
    }
    vector<int> wid(n, -1);
    for (int i = 0; i + L <= n; i++) {
        auto it = id.find(s.substr(i, L));
        if (it != id.end()) wid[i] = it->second;
    }
    vector<int> cnt(need.size(), 0);
    for (int off = 0; off < L; off++) {
        int left = off, used = 0;
        for (int j = off; j + L <= n; j += L) {
            int w = wid[j];
            if (w < 0) {
                while (left < j) { cnt[wid[left]]--; left += L; }
                used = 0; left = j + L;
                continue;
            }
            cnt[w]++; used++;
            while (cnt[w] > need[w]) { cnt[wid[left]]--; used--; left += L; }
            if (used == K) { res.push_back(left); cnt[wid[left]]--; used--; left += L; }
        }
        while (left + L <= n && used > 0) { cnt[wid[left]]--; used--; left += L; }
    }
    sort(res.begin(), res.end());
    return res;
}
