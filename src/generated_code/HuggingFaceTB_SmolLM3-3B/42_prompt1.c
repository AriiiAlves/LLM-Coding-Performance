#include <stdio.h>

int trap(int* height, int heightSize) {
    if (heightSize == 0) return 0;
    int left_max = height[0];
    int right_max = height[heightSize - 1];
    int water = 0;
    int i;

    for (i = 1; i < heightSize - 1; i++) {
        if (height[i] < left_max && height[i] < right_max) {
            water += (left_max > right_max ? right_max : left_max) - height[i];
        } else if (height[i] >= left_max) {
            left_max = height[i];
        } else {
            right_max = height[i];
        }
    }

    return water;
}

int main() {
    int height[] = {0,1,0,2,1,0,1,3,2,1,2,1};
    int heightSize = sizeof(height) / sizeof(height[0]);
    printf("%d\n", trap(height, heightSize));
    return 0;
}
