#include <stdio.h>

int reverse(int x) {
    long long res = 0;
    while (x != 0) {
        res = res * 10 + x % 10;
        x /= 10;
    }
    return (int)res == res ? (int)res : 0;
}

int main() {
    int x;
    scanf("%d", &x);
    printf("%d\n", reverse(x));
    return 0;
}
