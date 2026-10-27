#include <stdio.h>

bool isPalindrome(int x) {
    int reversed = 0;
    int original = x;

    while (x > 0) {
        reversed = reversed * 10 + x % 10;
        x /= 10;
    }

    return original == reversed;
}

int main() {
    printf("%d\n", isPalindrome(121)); // Output: 1
    printf("%d\n", isPalindrome(-121)); // Output: 0
    printf("%d\n", isPalindrome(10));  // Output: 0
    return 0;
}
