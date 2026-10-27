#include <stdio.h>

int reverse(int x) {
    int sign = x < 0 ? -1 : 1;
    x = abs(x);
    int reversed = 0;
    while (x > 0) {
        reversed = reversed * 10 + x % 10;
        x /= 10;
    }
    return sign * reversed;
}

int main() {
    int x;
    printf("Enter a signed 32-bit integer: ");
    scanf("%d", &x);
    int result = reverse(x);
    printf("Reversed integer: %d\n", result);
    return 0;
}
