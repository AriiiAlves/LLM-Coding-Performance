#include <string>
#include <stack>
#include <algorithm>
using namespace std;

int longestValidParentheses(string s) {
    stack<int> st;
    st.push(-1);
    int max_len = 0;
    for (int i = 0; i < s.length(); ++i) {
        if (s[i] == '(') {
            st.push(i);
        } else {
            st.pop();
            if (st.empty()) {
                st.push(i);
            } else {
                int current_len = i - st.top();
                max_len = max(max_len, current_len);
            }
        }
    }
    return max_len;
}
