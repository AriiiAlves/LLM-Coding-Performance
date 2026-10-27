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
            char* word = &s[start];
            int found = 0;

            for (int k = 0; k < wordCount; k++) {
                if (strcmp(word, words[k]) == 0 && wordCountMap[k] < 1) {
                    wordCountMap[k]++;
                    found = 1;
                    break;
                }
            }

            if (!found) {
                break;
            }
        }

        if (matchCount == wordCount) {
            result[resultIndex++] = i;
        }
    }

    *returnSize = resultIndex;
    return result;
}

int main() {
    char s[] = "barfoothefoobarman";
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
