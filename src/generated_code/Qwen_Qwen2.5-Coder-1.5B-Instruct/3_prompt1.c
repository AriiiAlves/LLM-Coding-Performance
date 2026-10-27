int lengthOfLongestSubstring(char* s) {
    int n = strlen(s);
    if(n == 0)
        return 0;
    
    int maxLen = 0;
    int start = 0;
    int charIndex[128] = {0};
    
    for(int end = 0; end < n; end++) {
        if(charIndex[s[end]] >= start) {
            start = charIndex[s[end]] + 1;
        }
        charIndex[s[end]] = end;
        maxLen = fmax(maxLen, end - start + 1);
    }
    
    return maxLen;
}
