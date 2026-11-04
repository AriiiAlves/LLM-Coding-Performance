#include <string>
#include <stack>
using namespace std;

int longestValidParentheses(string s) {
    stack<int> st;
    int maxLen = 0, last = -1;
    for (int i = 0; i < s.size(); ++i) {
        if (s[i] == '(') {
            st.push(i);
        } else {
            if (!st.empty()) {
                st.pop();
                if (st.empty()) {
                    maxLen = max(maxLen, i - last);
                } else {
                    maxLen = max(maxLen, i - st.top());
                }
            } else {
                last = i;
            }
        }
    }
    return maxLen;
}
