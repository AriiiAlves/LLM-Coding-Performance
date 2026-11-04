#include <vector>
#include <algorithm>
using namespace std;

double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2) {
    int totalLength = nums1.size() + nums2.size();
    if (totalLength % 2 == 1) {
        return findKthElement(nums1, nums2, totalLength / 2 + 1);
    } else {
        return (findKthElement(nums1, nums2, totalLength / 2) + findKthElement(nums1, nums2, totalLength / 2 + 1)) / 2.0;
    }
}

int findKthElement(vector<int>& nums1, vector<int>& nums2, int k) {
    int index1 = 0, index2 = 0;
    int len1 = nums1.size(), len2 = nums2.size();
    
    while (true) {
        if (index1 == len1) {
            return nums2[index2 + k - 1];
        }
        if (index2 == len2) {
            return nums1[index1 + k - 1];
        }
        if (k == 1) {
            return min(nums1[index1], nums2[index2]);
        }
        
        int half = k / 2;
        int newIndex1 = min(index1 + half, len1) - 1;
        int newIndex2 = min(index2 + half, len2) - 1;
        
        if (nums1[newIndex1] < nums2[newIndex2]) {
            k -= (newIndex1 - index1 + 1);
            index1 = newIndex1 + 1;
        } else {
            k -= (newIndex2 - index2 + 1);
            index2 = newIndex2 + 1;
        }
    }
}
