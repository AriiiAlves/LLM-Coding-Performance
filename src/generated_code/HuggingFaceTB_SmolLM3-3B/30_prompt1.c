#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    int words_len;
    scanf("%d", &words_len);
    Word *words = (Word *)malloc(words_len * sizeof(Word));
    for (int i = 0; i < words_len; i++) {
        scanf("%s", words[i].word);
    }
    qsort(words, words_len, sizeof(Word), compare);

    int s_len = strlen(s);
    int result[s_len];
    int count = 0;

    for (int i = 0; i <= s_len - words_len * MAX_WORD_LENGTH; i++) {
        int match = 1;
        Word *current = words;
        for (int j = 0; j < words_len; j++) {
            int pos = i + j * MAX_WORD_LENGTH;
            if (strncmp(s + pos, current->word, MAX_WORD_LENGTH) != 0) {
                match = 0;
                break;
            }
            current++;
        }
        if (match) {
            result[count++] = i;
        }
    }

    for (int i = 0; i < count; i++) {
        printf("%d ", result[i]);
    }
    printf("\n");

    free(words);
    return 0;
}
