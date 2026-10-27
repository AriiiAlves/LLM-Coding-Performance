#include <stdio.h>

/**
 * Note: The returned array must be malloced, assume the caller calls free().
 */
int* twoSum(int* nums, int numsSize, int target, int* returnSize) {
    int i, j;
    for (i = 0; i < numsSize; i++) {
        for (j = i + 1; j < numsSize; j++) {
            if (nums[i] + nums[j] == target) {
                *returnSize = 2;
                int* result = (int*)malloc(sizeof(int) * *returnSize);
                result[0] = i;
                result[1] = j;
                return result;
            }
        }
    }
    return NULL;
}

int main() {
    int nums[] = {2, 7, 11, 15};
    int target = 9;
    int returnSize;
    int* result = twoSum(nums, sizeof(nums) / sizeof(nums[0]), target, &returnSize);
    if (result != NULL) {
        printf("[%d, %d]", result[0], result[1]);
        free(result);
    } else {
        printf("No solution found.");
    }
    return 0;
}
