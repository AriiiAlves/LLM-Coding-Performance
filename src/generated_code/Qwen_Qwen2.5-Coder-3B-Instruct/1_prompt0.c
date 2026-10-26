#include <stdio.h>
#include <stdlib.h>

int* twoSum(int* nums, int numsSize, int target, int* returnSize) {
    int *result = (int *)malloc(2 * sizeof(int));
    for (int i = 0; i < numsSize; i++) {
        for (int j = i + 1; j < numsSize; j++) {
            if (nums[i] + nums[j] == target) {
                result[0] = i;
                result[1] = j;
                *returnSize = 2;
                return result;
            }
        }
    }
    *returnSize = 0;
    return NULL;
}

int main() {
    int numsSize, target, *nums, *result;
    int returnSize;

    // Read input
    scanf("%d", &numsSize);
    nums = (int *)malloc(numsSize * sizeof(int));
    for (int i = 0; i < numsSize; i++) {
        scanf("%d", &nums[i]);
    }
    scanf("%d", &target);

    // Call the function
    result = twoSum(nums, numsSize, target, &returnSize);

    // Print the result
    if (returnSize > 0) {
        printf("[%d, %d]\n", result[0], result[1]);
    } else {
        printf("No solution found\n");
    }

    // Free allocated memory
    free(nums);
    free(result);

    return 0;
}
