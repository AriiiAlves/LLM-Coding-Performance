#include <vector>
#include <string>
#include <unordered_map>
using namespace std;

vector<int> findSubstring(string s, vector<string>& words) {
    vector<int> result;
    if (s.empty() || words.empty()) return result;

    int wordCount = words.size();
    int wordLength = words[0].size();
    int totalLength = wordCount * wordLength;
    unordered_map<string, int> wordMap;

    for (const auto& word : words) {
        ++wordMap[word];
    }

    for (int i = 0; i <= s.length() - totalLength; ++i) {
        unordered_map<string, int> seenWords;
        int j = 0;
        while (j < wordCount) {
            string currentWord = s.substr(i + j * wordLength, wordLength);
            if (wordMap.find(currentWord) != wordMap.end()) {
                ++seenWords[currentWord];
                if (seenWords[currentWord] > wordMap[currentWord]) break;
            } else break;
            ++j;
        }
        if (j == wordCount) result.push_back(i);
    }

    return result;
}
