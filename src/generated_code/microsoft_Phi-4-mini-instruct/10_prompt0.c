#include <stdio.h>
#include <string.h>

int isMatch(char* s, char* p) {
    int s_len = strlen(s);
    int p_len = strlen(p);
    int s_idx = 0, p_idx = 0;

    while (s_idx < s_len) {
        if (p_idx < p_len && (p[p_idx] == s[s_idx] || p[p_idx] == '.')) {
            s_idx++;
            p_idx++;
        } else if (p_idx + 1 < p_len && p[p_idx] == '*' && (p[p_idx + 1] == s[s_idx] || p[p_idx + 1] == '.')) {
            p_idx += 2;
            s_idx++;
        } else {
            return 0;
        }
    }

    while (p_idx < p_len && p[p_idx] == '*') {
        p_idx++;
    }

    return p_idx == p_len;
}

int main() {
    char s[] = "ab";
    char p[] = ".*";
    printf("%d\n", isMatch(s, p));
    return 0;
}
