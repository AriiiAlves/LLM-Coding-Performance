#include <stdio.h>
#include <string.h>

#define MAX_WORDS 5000
#define WORD_LEN 30

int* findSubstring(char *s, char **words, int wordsSize, int* returnSize) {
    int wordCount = wordsSize;
    int wordLen = strlen(words[0]);
    int totalLen = wordCount * wordLen;
    int* result = (int*)malloc(10000 * sizeof(int));
    int resultIndex = 0;

    for (int i = 0; i <= strlen(s) - totalLen; i++) {
        int* wordCountMap = (int*)calloc(wordCount, sizeof(int));
        int matchCount = 0;

        for (int j = 0; j < wordCount; j++) {
            int start = i + j * wordLen;
            int end = start + wordLen;
            char* word = s + start;

            if (strstr(words[j], word) != NULL) {
                wordCountMap[j]++;
                matchCount++;
            }
        }

        if (matchCount == wordCount) {
            result[resultIndex++] = i;
        }

        free(wordCountMap);
    }

    *returnSize = resultIndex;
    return result;
}

int main() {
    char* s = "barfoothefoobarman";
    char* words[] = {"foo", "bar"};
    int wordsSize = 2;
    int returnSize;

    int* result = findSubstring(s, words, wordsSize, &returnSize);

    for (int i = 0; i < returnSize; i++) {
        printf("%d ", result[i]);
    }

    free(result);
    return 0;
}
