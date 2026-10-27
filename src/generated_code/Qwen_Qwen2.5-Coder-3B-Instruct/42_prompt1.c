#include <stdio.h>

int trap(int* height, int heightSize) {
    if (heightSize < 3) return 0;

    int leftMax[heightSize], rightMax[heightSize];
    leftMax[0] = height[0];
    rightMax[heightSize - 1] = height[heightSize - 1];

    for (int i = 1; i < heightSize; i++) {
        leftMax[i] = fmax(leftMax[i - 1], height[i]);
    }

    for (int i = heightSize - 2; i >= 0; i--) {
        rightMax[i] = fmax(rightMax[i + 1], height[i]);
    }

    int totalWater = 0;
    for (int i = 1; i < heightSize - 1; i++) {
        totalWater += fmin(leftMax[i], rightMax[i]) - height[i];
    }

    return totalWater;
}

int main() {
    int height[] = {0, 1, 0, 2, 1, 0, 1, 3, 2, 1, 2, 1};
    printf("%d\n", trap(height, sizeof(height) / sizeof(height[0])));
    return 0;
}
