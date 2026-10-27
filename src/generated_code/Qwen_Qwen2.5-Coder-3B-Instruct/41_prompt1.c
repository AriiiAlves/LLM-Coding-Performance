#include <stdio.h>

int firstMissingPositive(int* nums, int numsSize) {
    int i;
    
    // Place each number in its correct position if possible
    for (i = 0; i < numsSize; i++) {
        while (nums[i] > 0 && nums[i] <= numsSize && nums[nums[i] - 1] != nums[i]) {
            int temp = nums[nums[i] - 1];
            nums[nums[i] - 1] = nums[i];
            nums[i] = temp;
        }
    }
    
    // Find the first position where the number is not in its correct place
    for (i = 0; i < numsSize; i++) {
        if (nums[i] != i + 1) {
            return i + 1;
        }
    }
    
    // If all positions are correct, the missing number is numsSize + 1
    return numsSize + 1;
}

int main() {
    int nums[] = {3, 4, -1, 1};
    int numsSize = sizeof(nums) / sizeof(nums[0]);
    printf("%d\n", firstMissingPositive(nums, numsSize));
    return 0;
}
