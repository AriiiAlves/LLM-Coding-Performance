#include <stdio.h>

int findSmallestMissingPositive(int* nums, int numsSize) {
    int i = 0;
    while (i < numsSize) {
        int num = nums[i];
        if (num > 0 && num <= numsSize && nums[num - 1] != num) {
            // Swap the current number with the number at its correct position
            int temp = nums[num - 1];
            nums[num - 1] = num;
            nums[i] = temp;
        } else {
            i++;
        }
    }
    for (i = 0; i < numsSize; i++) {
        if (nums[i] != i + 1) {
            return i + 1;
        }
    }
    return numsSize + 1;
}

int main() {
    int nums[] = {3, 4, -1, 1};
    int result = findSmallestMissingPositive(nums, 4);
    printf("%d\n", result);
    return 0;
}
