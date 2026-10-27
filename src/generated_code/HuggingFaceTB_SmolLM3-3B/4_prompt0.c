#include <stdio.h>
#include <math.h>

double findMedianSortedArrays(int* nums1, int m, int* nums2, int n) {
    int left = 0, right = m + n;
    while (left < right) {
        int pivot1 = left + (right - left) / 2;
        int pivot2 = (m + n + 1) / 2 - 1;

        // Check if the partition is correct
        if (pivot1 > n) {
            right = pivot1;
        } else if (pivot2 > m) {
            left = pivot2;
        } else {
            // Calculate the max of the left partitions
            int maxLeft1 = (pivot1 == 0) ? INT_MIN : nums1[pivot1 - 1];
            int maxLeft2 = (pivot2 == 0) ? INT_MIN : nums2[pivot2 - 1];

            // Calculate the min of the right partitions
            int minRight1 = (pivot1 == m) ? INT_MAX : nums1[pivot1];
            int minRight2 = (pivot2 == n) ? INT_MAX : nums2[pivot2];

            // Check if the partition is correct
            if (maxLeft1 <= minRight2 && maxLeft2 <= minRight1) {
                // Calculate the median
                double median = 0.0;
                if ((m + n) % 2 == 0) {
                    median = (maxLeft1 + minRight1) / 2.0;
                } else {
                    median = (double) maxLeft1;
                }
                return median;
            } else if (maxLeft1 > minRight2) {
                right = pivot1;
            } else {
                left = pivot2;
            }
        }
    }

    // If the partition is correct, return the median
    if ((m + n) % 2 == 0) {
        double median = (double) nums1[left - 1] + nums2[left - 1];
        median /= 2.0;
        return median;
    } else {
        return (double) nums1[left - 1];
    }
}

int main() {
    int nums1[] = {1, 3};
    int m = 2;
    int nums2[] = {2};
    int n = 1;
    double median = findMedianSortedArrays(nums1, m, nums2, n);
    printf("%.5f\n", median);
    return 0;
}
