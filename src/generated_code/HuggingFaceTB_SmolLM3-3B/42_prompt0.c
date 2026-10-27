#include <stdio.h>

int trap(int* height, int heightSize) {
    if (heightSize == 0) return 0;
    int left = 0, right = heightSize - 1;
    int max_left = 0, max_right = 0, water = 0;
    while (left < right) {
        if (height[left] < max_left) {
            water += max_left - height[left];
        } else {
            max_left = height[left];
        }
        if (height[right] < max_right) {
            water += height[right] - max_right;
        } else {
            max_right = height[right];
        }
        if (height[left] < height[right]) {
            left++;
        } else {
            right--;
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
