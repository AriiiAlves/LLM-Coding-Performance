int trap(int* height, int heightSize) {
    if(heightSize < 3)
        return 0;
    
    int leftMax[heightSize], rightMax[heightSize];
    leftMax[0] = height[0];
    for(int i=1; i<heightSize; i++)
        leftMax[i] = max(leftMax[i-1], height[i]);
    
    rightMax[heightSize-1] = height[heightSize-1];
    for(int i=heightSize-2; i>=0; i--)
        rightMax[i] = max(rightMax[i+1], height[i]);
    
    int sum = 0;
    for(int i=1; i<heightSize-1; i++)
        sum += min(leftMax[i], rightMax[i]) - height[i];
    
    return sum;
}
