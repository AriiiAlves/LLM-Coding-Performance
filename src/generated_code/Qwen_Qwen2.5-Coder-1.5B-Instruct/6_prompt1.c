#include <stdio.h>
#include <stdlib.h>

char* convert(char* s, int numRows) {
    if(numRows == 1)
        return s;
    
    char** arr = (char**)malloc(sizeof(char*) * numRows);
    for(int i = 0; i < numRows; i++)
        arr[i] = (char*)malloc(sizeof(char) * strlen(s));
    
    int row = 0, col = 0;
    bool down = true;
    
    for(int i = 0; i < strlen(s); i++) {
        arr[row][col++] = s[i];
        
        if(row == 0 || row == numRows - 1)
            down = !down;
        
        if(down)
            row++;
        else
            row--;
    }
    
    char* res = (char*)malloc(sizeof(char) * (numRows * strlen(s)));
    col = 0;
    
    for(int i = 0; i < numRows; i++) {
        for(int j = 0; j < strlen(arr[i]); j++) {
            res[col++] = arr[i][j];
        }
    }
    
    for(int i = 0; i < numRows; i++)
        free(arr[i]);
    free(arr);
    
    return res;
}
