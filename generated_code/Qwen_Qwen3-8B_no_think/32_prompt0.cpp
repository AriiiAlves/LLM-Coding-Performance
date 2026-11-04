#include <string>
#include <stack>
using namespace std;

int longestValidParentheses(string s) {
    stack<int> indices;
    int maxLen = 0;
    int lastInvalid = -1;

    for (int i = 0; i < s.length(); ++i) {
        if (s[i] == '(') {
            indices.push(i);
        } else {
            if (!indices.empty()) {
                indices.pop();
                if (indices.empty()) {
                    maxLen = max(maxLen, i - lastInvalid);
                } else {
                    maxLen = max(maxLen, i - indices.top());
                }
            } else {
                lastInvalid = i;
            }
        }
    }

    return maxLen;
}
