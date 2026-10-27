#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int* findSubstring(char *s, char **words, int wordsSize, int *returnSize) {
    int n = strlen(s), m = wordsSize, wlen = strlen(words[0]), ans = 0;
    int count[m], i, j, k;

    // Initialize count array for each word
    memset(count, 0, sizeof(count));
    for (i = 0; i < m; i++)
        count[i] = 0;

    // Count occurrences of each word in the first window
    for (i = 0; i < wlen && i < n; i++)
        count[s[i] - 'a']++;

    // Slide the window across the string
    for (i = 0; i <= n - wlen * m; i++) {
        int tempCount[m];
        memcpy(tempCount, count, sizeof(count));

        // Check if the current window has the required words
        for (j = 0; j < m; j++) {
            k = i + j * wlen;
            if (tempCount[s[k] - 'a'] == 0)
                break;
            tempCount[s[k] - 'a']--;
        }

        // If all words are found, add the start index to the result
        if (j == m)
            ans++;
    }

    *returnSize = ans;
    return ans ? (int*)malloc(ans * sizeof(int)) : NULL;
}

int main() {
    char *s = "barfoothefoobarman";
    char *words[] = {"foo","bar"};
    int wordsSize = sizeof(words) / sizeof(words[0]);
    int returnSize;

    int *result = findSubstring(s, words, wordsSize, &returnSize);
    for (int i = 0; i < returnSize; i++)
        printf("%d ", result[i]);

    free(result);
    return 0;
}
