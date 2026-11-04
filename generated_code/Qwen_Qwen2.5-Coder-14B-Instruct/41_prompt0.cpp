#include <vector>
#include <algorithm>
using namespace std;

int firstMissingPositive(vector<int>& nums) {
    int n = nums.size();
    
    // Step 1: Place each number in its right place if possible
    for (int i = 0; i < n; ++i) {
        while (nums[i] > 0 && nums[i] <= n && nums[nums[i] - 1] != nums[i]) {
            swap(nums[i], nums[nums[i] - 1]);
        }
    }
    
    // Step 2: Find the first index where the number is not i + 1
    for (int i = 0; i < n; ++i) {
        if (nums[i] != i + 1) {
            return i + 1;
        }
    }
    
    // If all numbers are in their correct place, return n + 1
    return n + 1;
}
