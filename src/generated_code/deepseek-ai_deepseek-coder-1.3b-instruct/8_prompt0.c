#include <stdio.h>
#include <ctype.h>
#include <limits.h>

int myAtoi(char *s) {
    int sign = 1;
    int result = 0;
    char *start = s;

    // Skip whitespace
    while (*s == ' ') {
        s++;
    }

    // Check sign
    if (*s == '-') {
        sign = -1;
        s++;
    } else if (*s == '+') {
        s++;
    }

    // Read digits
    while (isdigit(*s)) {
        result = result * 10 + (*s - '0');
        s++;
    }

    // Check overflow
    if (result > INT_MAX || result < (sign == -1 ? INT_MIN : 0)) {
        return sign == -1 ? INT_MIN : INT_MAX;
    }

    return result * sign;
}

int main() {
    printf("%d\n", myAtoi("42"));
    printf("%d\n", myAtoi(" -042"));
    printf("%d\n", myAtoi("1337c0d3"));
    printf("%d\n", myAtoi("0-1"));
    printf("%d\n", myAtoi("words and 987"));
    return 0;
}
