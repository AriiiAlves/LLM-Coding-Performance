#include <stdbool.h>
#include <stdio.h>

bool isPalindrome(int x) {
    if (x < 0 || (x % 10 == 0 && x != 0)) {
        return false;
    }

    int reversedNumber = 0;
    while (x > reversedNumber) {
        reversedNumber = reversedNumber * 10 + x % 10;
        x /= 10;
    }

    return x == reversedNumber || x == reversedNumber / 10;
}

int main() {
    int x = 121;
    if (isPalindrome(x)) {
        printf("true\n");
    } else {
        printf("false\n");
    }

    return 0;
}
