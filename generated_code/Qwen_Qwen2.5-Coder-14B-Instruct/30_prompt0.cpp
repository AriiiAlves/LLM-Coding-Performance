#include <vector>
#include <string>
#include <unordered_map>
using namespace std;

vector<int> findSubstring(string s, vector<string>& words) {
    vector<int> result;
    if (words.empty() || s.empty()) return result;

    int wordLen = words[0].size();
    int numWords = words.size();
    int totalLen = wordLen * numWords;

    unordered_map<string, int> wordCount;
    for (const string& word : words) {
        wordCount[word]++;
    }

    for (int i = 0; i <= (int)s.size() - totalLen; ++i) {
        unordered_map<string, int> seenWords;
        int j = 0;
        for (; j < numWords; ++j) {
            string word = s.substr(i + j * wordLen, wordLen);
            if (wordCount.find(word) == wordCount.end()) break;
            seenWords[word]++;
            if (seenWords[word] > wordCount[word]) break;
        }
        if (j == numWords) result.push_back(i);
    }

    return result;
}
