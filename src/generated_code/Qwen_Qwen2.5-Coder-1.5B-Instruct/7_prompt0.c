int reverse(int x) {
    int sign = x < 0 ? -1 : 1;
    long long res = 0;

    while (x != 0) {
        res = res * 10 + x % 10;
        if (res > INT_MAX || res < INT_MIN)
            return 0;
        x /= 10;
    }

    return sign * res;
}
