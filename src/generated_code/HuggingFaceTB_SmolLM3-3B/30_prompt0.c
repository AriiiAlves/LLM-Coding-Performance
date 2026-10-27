#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_WORD_LENGTH 30
#define MAX_WORDS 5000
#define MAX_S 10000

typedef struct {
    char word[MAX_WORD_LENGTH + 1];
} Word;

int compare(const void *a, const void *b) {
    return strcmp(((Word *)a)->word, ((Word *)b)->word);
}

int main() {
    char s[MAX_S + 1];
    scanf("%s", s);
    int n = strlen(s);
    int m;
    scanf("%d", &m);
    Word *words = (Word *)malloc(m * sizeof(Word));
    for (int i = 0; i < m; i++) {
        scanf("%s", words[i].word);
    }
    qsort(words, m, sizeof(Word), compare);

    int *result = (int *)malloc(n * sizeof(int));
    int count = 0;
    for (int i = 0; i < n - m * words[0].length + 1; i++) {
        bool found = true;
        int *counts = (int *)calloc(words[0].length, sizeof(int));
        for (int j = 0; j < m; j++) {
            int k = i + j * words[0].length;
            int *temp = (int *)calloc(words[0].length, sizeof(int));
            for (int l = 0; l < words[0].length; l++) {
                temp[l] = (s[k + l] - 'a' + 1);
            }
            for (int l = 0; l < words[0].length; l++) {
                counts[temp[l] - 1]++;
            }
        }
        for (int j = 0; j < m; j++) {
            int k = i + j * words[0].length;
            int *temp = (int *)calloc(words[0].length, sizeof(int));
            for (int l = 0; l < words[0].length; l++) {
                temp[l] = (s[k + l] - 'a' + 1);
            }
            for (int l = 0; l < words[0].length; l++) {
                counts[temp[l] - 1]--;
            }
            for (int l = 0; l < words[0].length; l++) {
                if (counts[temp[l] - 1] < 0) {
                    found = false;
                    break;
                }
            }
            if (!found) break;
            for (int l = 0; l < words[0].length; l++) {
                counts[temp[l] - 1]++;
            }
        }
        if (found) {
            result[count++] = i;
        }
        free(counts);
    }
    for (int i = 0; i < count; i++) {
        printf("%d ", result[i]);
    }
    printf("\n");
    free(result);
    free(words);
    return 0;
}
