#include <stdio.h>

double findMedianSortedArrays(int* nums1, int nums1Size, int* nums2, int nums2Size) {
    int totalSize = nums1Size + nums2Size;
    int *mergedArray = (int *)malloc(totalSize * sizeof(int));
    
    int i = 0, j = 0, k = 0;
    while (i < nums1Size && j < nums2Size) {
        if (nums1[i] < nums2[j]) {
            mergedArray[k++] = nums1[i++];
        } else {
            mergedArray[k++] = nums2[j++];
        }
    }
    
    while (i < nums1Size) {
        mergedArray[k++] = nums1[i++];
    }
    
    while (j < nums2Size) {
        mergedArray[k++] = nums2[j++];
    }
    
    double median;
    if (totalSize % 2 == 0) {
        median = (mergedArray[totalSize / 2 - 1] + mergedArray[totalSize / 2]) / 2.0;
    } else {
        median = mergedArray[totalSize / 2];
    }
    
    free(mergedArray);
    return median;
}

int main() {
    int nums1[] = {1, 3};
    int nums2[] = {2};
    printf("%.2f\n", findMedianSortedArrays(nums1, 2, nums2, 1)); // Output: 2.00
    
    int nums3[] = {1, 2};
    int nums4[] = {3, 4};
    printf("%.2f\n", findMedianSortedArrays(nums3, 2, nums4, 2)); // Output: 2.50
    
    return 0;
}
