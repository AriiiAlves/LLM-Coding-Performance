#include <stdio.h>
#include <stdlib.h>

char* convert(char* s, int numRows) {
    if(numRows == 1)
        return s;
    
    char** zigzag = (char**)malloc(sizeof(char*) * numRows);
    for(int i = 0; i < numRows; i++)
        zigzag[i] = (char*)malloc(sizeof(char) * strlen(s));
    
    int row = 0, col = 0;
    bool goingDown = true;
    
    for(int i = 0; i < strlen(s); i++) {
        zigzag[row][col++] = s[i];
        
        if(goingDown && row == numRows - 1 || !goingDown && row == 0) {
            goingDown = !goingDown;
        }
        
        if(goingDown)
            row++;
        else
            row--;
    }
    
    char* result = (char*)malloc(sizeof(char) * (numRows * strlen(s)));
    col = 0;
    
    for(int i = 0; i < numRows; i++) {
        for(int j = 0; j < strlen(zigzag[i]); j++) {
            result[col++] = zigzag[i][j];
        }
    }
    
    for(int i = 0; i < numRows; i++)
        free(zigzag[i]);
    free(zigzag);
    
    return result;
}
