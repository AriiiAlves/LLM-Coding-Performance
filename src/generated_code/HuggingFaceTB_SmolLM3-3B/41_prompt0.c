#include <stdio.h>

int firstMissingPositive(int* nums, int numsSize) {
    for (int i = 0; i < numsSize; i++) {
        if (nums[i] <= 0 || nums[i] > numsSize) {
            nums[i] = 0;
        } else {
            int index = nums[i] - 1;
            if (nums[index] != index + 1) {
                int temp = nums[index];
                nums[index] = index + 1;
                while (nums[index] != index + 1) {
                    int nextIndex = nums[index] - 1;
                    if (nums[nextIndex] == nextIndex + 1) {
                        break;
                    }
                    int temp2 = nums[nextIndex];
                    nums[nextIndex] = nextIndex + 1;
                    nums[index] = temp2;
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
