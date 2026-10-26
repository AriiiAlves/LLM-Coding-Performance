#include <stdio.h>

int* twoSum(int* nums, int numsSize, int target, int* returnSize) {
    *returnSize = 2;
    int i, j;

    for(i = 0; i < numsSize - 1; i++){
        for(j = i + 1; j < numsSize; j++){
            if(nums[i] + nums[j] == target){
                int* result = (int*)malloc(2 * sizeof(int));
                result[0] = i;
                result[1] = j;
                return result;
            }
        }
    }

    return NULL;
}

int main(){
    int nums[] = {2,7,11,15};
    int target = 9;
    int size;
    int* result = twoSum(nums, sizeof(nums)/sizeof(nums[0]), target, &size);

    printf("%d %d\n", result[0], result[1]);

    free(result);
    return 0;
}
