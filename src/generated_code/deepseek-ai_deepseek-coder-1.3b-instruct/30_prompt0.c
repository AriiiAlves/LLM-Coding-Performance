#include <stdio.h>
#include <string.h>

int* findSubstring(char* s, int* returnSize, char** words, int wordsSize, int* wordsColSize) {
    if (s == NULL || words == NULL || returnSize == NULL || wordsColSize == NULL) {
        *returnSize = 0;
        return NULL;
    }

    int sLen = strlen(s);
    int wordLen = words[0][0] == '\0' ? 0 : strlen(words[0]);
    int totalLen = wordLen * wordsSize;
    int count = 0;
    int* result = malloc(sLen * sizeof(int));
    int* index = malloc(sLen * sizeof(int));
    memset(result, -1, sLen * sizeof(int));
    memset(index, -1, sLen * sizeof(int));

    for (int i = 0; i < sLen; i++) {
        if (result[i] != -1) continue;
        if (i + totalLen > sLen) break;
        if (strncmp(s + i, words[count], wordLen) != 0) continue;
        result[i] = count;
        for (int j = 0; j < wordsSize; j++) {
            index[i + j * wordLen] = count;
        }
        count++;
    }

    *returnSize = count;
    return result;
}

int main() {
    char* s = "barfoothefoobarman";
    int wordsSize = 2;
    char* words[2] = {"foo", "bar"};
    int returnSize;
    int* result = findSubstring(s, &returnSize, words, wordsSize, NULL);
    for (int i = 0; i < returnSize; i++) {
        printf("%d ", result[i]);
    }
    free(result);
    free(result);
    return 0;
}
