#include <stdio.h>

int reverse(int x) {
    int rev = 0;
    while (x != 0) {
        int pop = x % 10;
        x /= 10;
        if (rev > (231 - 1) / 10 || (rev == (231 - 1) / 10 && pop > 7)) return 0;
        if (rev < (-231) / 10 || (rev == (-231) / 10 && pop < -8)) return 0;
        rev = rev * 10 + pop;
    }
    return rev;
}

int main() {
    int x = -123; // Example input
    printf("%d\n", reverse(x));
    return 0;
}
