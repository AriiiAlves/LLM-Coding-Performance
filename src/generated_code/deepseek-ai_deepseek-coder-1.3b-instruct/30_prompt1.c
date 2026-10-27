#include <stdio.h>
#include <string.h>

int* findSubstring(char* s, int* returnSize, char** words, int wordsSize, int* wordsColSize) {
    if (!s || !words || wordsSize == 0 || wordsColSize == 0) {
        *returnSize = 0;
        return NULL;
    }

    int len = strlen(s);
    int wordLen = words[0][0] ? strlen(words[0]) : 0;
    int numWords = wordsSize / wordsColSize[0];
    int* result = malloc(sizeof(int) * len);
    memset(result, -1, sizeof(int) * len);
    *returnSize = 0;

    for (int i = 0; i < numWords; i++) {
        char* word = words[i * wordsColSize[0]];
        int wordIndex = 0;
        for (int j = i * wordsColSize[0]; j < len; j += wordLen) {
            if (strncmp(s + j, word, wordLen) == 0) {
                result[(*returnSize)++] = j;
                wordIndex++;
                word = words[wordIndex * wordsColSize[0]];
            }
        }
        if (wordIndex != wordsColSize[0]) {
            return result;
        }
    }

    return result;
}

int main() {
    char* s = "barfoothefoobarman";
    int wordsSize = 4;
    char* words[4] = {"foo", "bar"};
    int wordsColSize[4] = {2, 3};
    int returnSize = 0;
    int* result = findSubstring(s, &returnSize, words, wordsSize, wordsColSize);
    for (int i = 0; i < returnSize; i++) {
        printf("%d ", result[i]);
    }
    free(result);
    return 0;
}
