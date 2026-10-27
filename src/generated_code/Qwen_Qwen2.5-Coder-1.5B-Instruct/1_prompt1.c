#include <stdio.h>

int* twoSum(int* nums, int numsSize, int target, int* returnSize) {
    *returnSize = 2;
    for (int i = 0; i < numsSize - 1; i++) {
        for (int j = i + 1; j < numsSize; j++) {
            if (nums[i] + nums[j] == target) {
                printf("%d %d\n", i, j);
                return (int*)malloc(2 * sizeof(int));
                *(int*)malloc(2 * sizeof(int)) = i;
                *(int*)malloc(2 * sizeof(int)) = j;
                return (int*)malloc(2 * sizeof(int));
            }
        }
    }
    return NULL;
}
