#include <stdio.h>

int firstMissingPositive(int* nums, int numsSize) {
    for (int i = 0; i < numsSize; i++) {
        if (nums[i] > 0 && nums[i] <= numsSize) {
            int index = nums[i] - 1;
            if (nums[index] != index + 1) {
                int temp = nums[index];
                nums[index] = index + 1;
                while (nums[index] != index + 1 && nums[index] > 0 && nums[index] <= numsSize) {
                    int nextIndex = nums[index] - 1;
                    int nextValue = nums[nextIndex];
                    nums[index] = nextValue;
                    nums[nextIndex] = index + 1;
                    index = nextIndex;
                }
            }
        }
    }

    for (int i = 0; i < numsSize; i++) {
        if (nums[i] != i + 1) {
            return i + 1;
        }
    }

    return numsSize + 1;
}

int main() {
    int nums[] = {1, 2, 0};
    int numsSize = sizeof(nums) / sizeof(nums[0]);
    printf("%d\n", firstMissingPositive(nums, numsSize));
    return 0;
}
