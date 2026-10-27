class Solution {
public:
    vector<int> findSubstring(string s, vector<string>& words) {
        vector<int> res;
        if (s.empty() || words.empty()) return res;
        int wordLen = words[0].size();
        int numWords = words.size();
        int totalLen = wordLen * numWords;
        if (s.size() < totalLen) return res;

        unordered_map<string, int> wordCount;
        for (const string& w : words) wordCount[w]++;

        for (int i = 0; i < wordLen; ++i) {
            int left = i, right = i;
            unordered_map<string, int> seen;
            int count = 0;
            while (right + wordLen <= s.size()) {
                string sub = s.substr(right, wordLen);
                right += wordLen;
                if (wordCount.count(sub)) {
                    seen[sub]++;
                    if (seen[sub] <= wordCount[sub]) count++;
                    else {
                        while (seen[sub] > wordCount[sub]) {
                            string leftSub = s.substr(left, wordLen);
                            seen[leftSub]--;
                            if (seen[leftSub] < wordCount[leftSub]) count--;
                            left += wordLen;
                        }
                    }
                    if (count == numWords) {
                        res.push_back(left);
                        string leftSub = s.substr(left, wordLen);
                        seen[leftSub]--;
                        count--;
                        left += wordLen;
                    }
                } else {
                    seen.clear();
                    count = 0;
                    left = right;
                }
            }
        }
        return res;
    }
};