#include <stdio.h>

int* twoSum(int* nums, int numsSize, int target, int* returnSize) {
    int i, j;
    for (i = 0; i < numsSize - 1; i++) {
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
    *returnSize = 0;
    return NULL;
}

int main() {
    int nums[3], target, i, size;
    printf("Enter the number of elements: ");
    scanf("%d", &numsSize);
    printf("Enter the elements: ");
    for (i = 0; i < numsSize; i++) {
        scanf("%d", &nums[i]);
    }
    printf("Enter the target: ");
    scanf("%d", &target);
    int* result = twoSum(nums, numsSize, target, &size);
    if (size != 0) {
        printf("The result is: %d, %d\n", result[0], result[1]);
        free(result);
    } else {
        printf("No solution found.\n");
    }
    return 0;
}
