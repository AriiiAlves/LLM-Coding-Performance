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
        int digit = *s - '0';
        if (result > INT_MAX / 10 || (result == INT_MAX / 10 && digit > INT_MAX % 10)) {
            if (sign == 1) {
                return INT_MAX;
            } else {
                return INT_MIN;
            }
        }
        result = result * 10 + digit;
        s++;
    }

    return sign * result;
}

int main() {
    char s1[] = "42";
    printf("%d\n", myAtoi(s1));  // Output: 42

    char s2[] = " -042";
    printf("%d\n", myAtoi(s2));  // Output: -42

    char s3[] = "1337c0d3";
    printf("%d\n", myAtoi(s3));  // Output: 1337

    char s4[] = "0-1";
    printf("%d\n", myAtoi(s4));  // Output: 0

    char s5[] = "words and 987";
    printf("%d\n", myAtoi(s5));  // Output: 0

    return 0;
}
