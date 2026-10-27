#include <stdio.h>
#include <string.h>

int longestValidParentheses(char * s) {
    int max_length = 0;
    int current_length = 0;
    int i;

    for(i = 0; i < strlen(s); i++) {
        if(s[i] == '(') {
            current_length++;
        } else {
            current_length--;
            if(current_length == 0) {
                if(max_length < current_length) {
                    max_length = current_length;
                }
            } else if(current_length < 0) {
                current_length = 0;
            }
        }
    }

    return max_length;
}

int main() {
    char s[] = "(())";
    printf("%d\n", longestValidParentheses(s));
    return 0;
}
