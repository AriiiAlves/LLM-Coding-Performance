int myAtoi(char *s) {
    int i = 0;
    while (s[i] == ' ') i++;
    int sign = 1;
    if (s[i] == '-') {
        sign = -1;
        i++;
    } else if (s[i] == '+') i++;

    long res = 0;
    while (i < strlen(s)) {
        if (!isdigit(s[i])) break;

        res *= 10;
        res += s[i] - '0';
        if (res > INT_MAX) return sign == 1 ? INT_MAX : INT_MIN;
        i++;
    }

    return sign * res;
}
