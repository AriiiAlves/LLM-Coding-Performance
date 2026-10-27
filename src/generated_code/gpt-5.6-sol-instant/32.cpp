class Solution {
public:
    int longestValidParentheses(string s) {
        stack<int> st;

        // Base index before the current valid substring.
        st.push(-1);

        int answer = 0;

        for (int i = 0; i < (int)s.size(); ++i) {
            if (s[i] == '(') {
                st.push(i);
            }
            else {
                st.pop();

                if (st.empty()) {
                    // This ')' cannot participate in
                    // a valid substring before i.
                    st.push(i);
                }
                else {
                    answer = max(
                        answer,
                        i - st.top()
                    );
                }
            }
        }

        return answer;
    }
};