#include <stdio.h>
#include <string.h>

int lengthOfLongestSubstring(char * s) {
    int n = strlen(s);
    int res = 0;
    int i = 0, j = 0;
    int char_pos[256] = {0};

    while (i < n && j < n) {
        if (char_pos[s[j]] > 0) {
            char_pos[s[i]]--;
            i++;
        } else {
            char_pos[s[j]]++;
            j++;
            res = (res < j - i) ? j - i : res;
        }
    }

    return res;
}

int main() {
    char s[1000];
    scanf("%s", s);
    printf("%d\n", lengthOfLongestSubstring(s));
    return 0;
}
