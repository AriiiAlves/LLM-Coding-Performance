#include <stdio.h>
#include <string.h>

int lengthOfLongestSubstring(char * s) {
    int n = strlen(s);
    int i = 0, j = 0, max = 0;
    int used[256] = {0};

    while (i < n && j < n) {
        if (used[s[j]] == 0) {
            used[s[j]] = 1;
            j++;
            max = max > j - i ? max : j - i;
        } else {
            used[s[i]] = 0;
            i++;
        }
    }

    return max;
}

int main() {
    char s[1000];
    scanf("%s", s);
    printf("%d\n", lengthOfLongestSubstring(s));
    return 0;
}
