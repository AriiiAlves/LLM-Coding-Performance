#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int myAtoi(char *s) {
    int sign = 1;
    int i = 0;
    int num = 0;
    int max = 2147483647;
    int min = -2147483648;

    // Skip leading whitespace
    while (s[i] == ' ') {
        i++;
    }

    // Determine sign
    if (s[i] == '-') {
        sign = -1;
        i++;
    } else if (s[i] == '+') {
        i++;
    }

    // Read digits
    while (s[i] >= '0' && s[i] <= '9') {
        if (num > (max - s[i] - 48) / 10) {
            return sign == 1 ? max : min;
        }
        num = num * 10 + (s[i++] - '0');
    }

    return sign * num;
}

int main() {
    char s[] = "42";
    printf("%d\n", myAtoi(s));
    return 0;
}
