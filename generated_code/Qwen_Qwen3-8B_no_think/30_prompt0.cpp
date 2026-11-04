#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
using namespace std;

vector<int> findSubstring(string s, vector<string>& words) {
    int wordCount = words.size();
    int wordLen = words[0].size();
    int totalLen = wordCount * wordLen;
    vector<int> result;
    unordered_map<string, int> wordFreq;

    for (string word : words) {
        wordFreq[word]++;
    }

    for (int i = 0; i < wordLen; i++) {
        int left = i, right = i;
        unordered_map<string, int> windowFreq;
        int count = 0;

        while (right + wordLen <= s.size()) {
            string currWord = s.substr(right, wordLen);
            right += wordLen;

            if (wordFreq.find(currWord) != wordFreq.end()) {
                windowFreq[currWord]++;
                if (windowFreq[currWord] == wordFreq[currWord]) count++;
            } else {
                windowFreq.clear();
                count = 0;
                left = right;
            }

            while (right - left >= totalLen) {
                string leftWord = s.substr(left, wordLen);
                if (windowFreq[leftWord] > wordFreq[leftWord]) {
                    windowFreq[leftWord]--;
                } else {
                    windowFreq.erase(leftWord);
                    count--;
                }
                left += wordLen;

                if (count == wordCount) {
                    result.push_back(left);
                }
            }
        }
    }

    return result;
}
