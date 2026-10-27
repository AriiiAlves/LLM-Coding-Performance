class Solution {
public:
    vector<int> findSubstring(string s, vector<string>& words) {
        vector<int> result;

        if (s.empty() || words.empty())
            return result;

        int wordLength = words[0].size();
        int wordCount = words.size();

        unordered_map<string, int> required;

        for (const string& word : words)
            required[word]++;

        for (int offset = 0; offset < wordLength; ++offset) {
            int left = offset;
            int count = 0;

            unordered_map<string, int> current;

            for (int right = offset;
                 right + wordLength <= (int)s.size();
                 right += wordLength) {

                string word = s.substr(right, wordLength);

                if (required.count(word)) {
                    current[word]++;
                    count++;

                    // Too many occurrences of this word.
                    while (current[word] > required[word]) {
                        string leftWord =
                            s.substr(left, wordLength);

                        current[leftWord]--;
                        left += wordLength;
                        count--;
                    }

                    if (count == wordCount) {
                        result.push_back(left);

                        // Move window forward to search
                        // for overlapping answers.
                        string leftWord =
                            s.substr(left, wordLength);

                        current[leftWord]--;
                        left += wordLength;
                        count--;
                    }
                }
                else {
                    current.clear();
                    count = 0;
                    left = right + wordLength;
                }
            }
        }

        return result;
    }
};