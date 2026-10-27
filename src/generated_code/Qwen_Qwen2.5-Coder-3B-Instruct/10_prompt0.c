#include <stdio.h>
#include <stdbool.h>

bool isMatch(const char *s, const char *p) {
    if (*p == '\0') return *s == '\0';

    bool first_match = *s != '\0' && (*s == *p || *p == '.');
    if (*(p + 1) == '*') {
        return isMatch(s, p + 2) || (first_match && isMatch(s + 1, p));
    } else {
        return first_match && isMatch(s + 1, p + 1);
    }
}

int main() {
    const char *s1 = "aa";
    const char *p1 = "a";
    printf("%d\n", isMatch(s1, p1)); // Output: 0

    const char *s2 = "aa";
    const char *p2 = "a*";
    printf("%d\n", isMatch(s2, p2)); // Output: 1

    const char *s3 = "ab";
    const char *p3 = ".*";
    printf("%d\n", isMatch(s3, p3)); // Output: 1

    return 0;
}
