#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int myAtoi(char* s) {
    int sign = 1, base = 0, i = 0, n = strlen(s);
    while (i < n && isspace(s[i])) i++;
    if (i < n) {
        if (s[i] == '-') {
            sign = -1;
            i++;
        } else if (s[i] == '+') {
            i++;
        }
        while (i < n && isdigit(s[i])) {
            if (base > INT_MAX / 10 || (base == INT_MAX / 10 && s[i] - '0' > 7)) {
                return sign == 1 ? INT_MAX : INT_MIN;
            }
            base = base * 10 + (s[i] - '0');
            i++;
        }
    }
    return sign * base;
}

int main() {
    char s[] = "words and 987";
    printf("%d\n", myAtoi(s));
    return 0;
}
