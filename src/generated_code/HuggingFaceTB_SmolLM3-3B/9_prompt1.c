#include <stdbool.h>

bool isPalindrome(int x) {
    if (x < 0) return false;
    int reversed = 0;
    int original = x;
    while (x > 0) {
        reversed = reversed * 10 + x % 10;
        x /= 10;
    }
    return original == reversed;
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
