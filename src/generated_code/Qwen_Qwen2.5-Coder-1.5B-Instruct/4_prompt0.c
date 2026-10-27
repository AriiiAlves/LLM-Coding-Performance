#include <stdio.h>

double findMedianSortedArrays(int* nums1, int nums1Size, int* nums2, int nums2Size) {
    int totalLength = nums1Size + nums2Size;
    int leftIndex = 0, rightIndex = 0;
    double median;

    while (leftIndex + rightIndex < totalLength / 2 + 1) {
        int mid1 = leftIndex + (rightIndex - leftIndex) / 2;
        int mid2 = rightIndex + (totalLength - leftIndex - rightIndex) / 2;

        if (mid1 >= nums1Size) {
            median = nums2[mid2];
        } else if (mid2 >= nums2Size) {
            median = nums1[mid1];
        } else if (nums1[mid1] > nums2[mid2]) {
            rightIndex = mid1;
        } else {
            leftIndex = mid1 + 1;
        }
    }

    if (totalLength % 2 == 0) {
        median = (median + nums1[leftIndex] + nums2[rightIndex]) / 2.0;
    } else {
        median = nums1[leftIndex];
    }

    return median;
}

int main() {
    int nums1[] = {1, 3};
    int nums2[] = {2};
    printf("%.2f\n", findMedianSortedArrays(nums1, sizeof(nums1) / sizeof(nums1[0]), nums2, sizeof(nums2) / sizeof(nums2[0])));
    return 0;
}
