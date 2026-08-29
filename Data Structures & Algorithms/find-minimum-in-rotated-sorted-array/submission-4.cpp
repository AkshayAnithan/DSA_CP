class Solution {
   public:
    int findMin(vector<int>& nums) {
        int left = 0, right = nums.size() - 1;
        int res = nums[0];
        while (left <= right) {
            if (nums[left] < nums[right]) {
                res = min(res, nums[left]);
            }
            int middle = left + (right - left) / 2;
            res = min(res, nums[middle]);
            if (nums[middle] >= nums[left]) {
                left = middle + 1;
            } else {
                right = middle;
            }
        }
        return res;
    }
};
