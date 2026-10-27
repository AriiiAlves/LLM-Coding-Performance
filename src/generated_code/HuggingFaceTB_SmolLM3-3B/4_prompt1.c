#include <stdio.h>
#include <math.h>

double findMedianSortedArrays(int* nums1, int m, int* nums2, int n) {
    int left = 0, right = m + n - 1;
    while (left < right) {
        int pivot1 = left + (right - left) / 2;
        int pivot2 = (m + n + 1) / 2 - 1 - (pivot1 - left);
        if (nums1[pivot1] < nums2[pivot2]) {
            left = pivot1 + 1;
        } else if (nums1[pivot1] > nums2[pivot2]) {
            right = pivot1 - 1;
        } else {
            if ((m + n) % 2 == 0) {
                return (double)(nums1[pivot1] + nums2[pivot2]) / 2.0;
            } else {
                return (double)nums1[pivot1];
            }
        }
    }
    if ((m + n) % 2 == 0) {
        return (double)(nums1[left] + nums2[left]) / 2.0;
    } else {
        return (double)nums1[left];
    }
}

int main() {
    int nums1[] = {1, 3};
    int nums2[] = {2};
    int m = sizeof(nums1) / sizeof(nums1[0]);
    int n = sizeof(nums2) / sizeof(nums2[0]);
    printf("%.5f\n", findMedianSortedArrays(nums1, m, nums2, n));
    return 0;
}
