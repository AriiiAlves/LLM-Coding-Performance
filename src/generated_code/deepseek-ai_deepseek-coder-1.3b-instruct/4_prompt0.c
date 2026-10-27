#include <stdio.h>
#include <stdlib.h>

double findMedianSortedArrays(int* nums1, int nums1Size, int* nums2, int nums2Size) {
    int totalSize = nums1Size + nums2Size;
    int* mergedArray = (int*)malloc(totalSize * sizeof(int));
    
    int i = 0, j = 0, k = 0;
    while(i < nums1Size && j < nums2Size) {
        if(nums1[i] < nums2[j]) {
            mergedArray[k++] = nums1[i++];
        } else {
            mergedArray[k++] = nums2[j++];
        }
    }
    
    while(i < nums1Size) {
        mergedArray[k++] = nums1[i++];
    }
    
    while(j < nums2Size) {
        mergedArray[k++] = nums2[j++];
    }
    
    if(totalSize % 2 == 0) {
        return (mergedArray[totalSize / 2 - 1] + mergedArray[totalSize / 2]) / 2.0;
    } else {
        return mergedArray[totalSize / 2];
    }
}

int main() {
    int nums1[] = {1, 3};
    int nums2[] = {2};
    int nums1Size = sizeof(nums1) / sizeof(nums1[0]);
    int nums2Size = sizeof(nums2) / sizeof(nums2[0]);
    
    double result = findMedianSortedArrays(nums1, nums1Size, nums2, nums2Size);
    printf("Median: %.5f\n", result);
    
    return 0;
}
