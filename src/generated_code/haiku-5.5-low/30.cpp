class Solution {
public:
    vector<int> findSubstring(string s, vector<string>& words) {
        vector<int> result;
        if (words.empty() || s.empty()) return result;

        int wordLen = words[0].size();
        int numWords = words.size();
        int totalLen = wordLen * numWords;
        if ((int)s.size() < totalLen) return result;

        unordered_map<string, int> need;
        for (const string& w : words) need[w]++;

        for (int offset = 0; offset < wordLen; offset++) {
            int left = offset, count = 0;
            unordered_map<string, int> seen;

            for (int right = offset; right + wordLen <= (int)s.size(); right += wordLen) {
                string word = s.substr(right, wordLen);
                if (need.find(word) != need.end()) {
                    seen[word]++;
                    count++;
                    while (seen[word] > need[word]) {
                        string leftWord = s.substr(left, wordLen);
                        seen[leftWord]--;
                        count--;
                        left += wordLen;
                    }
                    if (count == numWords) {
                        result.push_back(left);
                        string leftWord = s.substr(left, wordLen);
                        seen[leftWord]--;
                        count--;
                        left += wordLen;
                    }
                } else {
                    seen.clear();
                    count = 0;
                    left = right + wordLen;
                }
            }
        }
        return result;
    }
};