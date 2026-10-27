#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int myAtoi(char *str) {
    int i = 0;
    int sign = 1;
    long result = 0;

    // Skip leading whitespace
    while (isspace(str[i])) {
        i++;
    }

    // Check for sign
    if (str[i] == '-') {
        sign = -1;
        i++;
    } else if (str[i] == '+') {
        i++;
    }

    // Convert digits
    while (isdigit(str[i])) {
        int digit = str[i] - '0';
        if (result > INT_MAX / 10 || (result == INT_MAX / 10 && digit > 7)) {
            return sign == 1 ? INT_MAX : INT_MIN;
        }
        result = result * 10 + digit;
        i++;
    }

    return result * sign;
}

int main() {
    char input[] = "   -042";
    printf("%d\n", myAtoi(input));
    return 0;
}
