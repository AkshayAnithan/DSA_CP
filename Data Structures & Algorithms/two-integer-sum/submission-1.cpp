class Solution {
   public:
    vector<int> twoSum(vector<int>& nums, int target) {
        unordered_map<int, int> hs;
        for (int i = 0; i < nums.size(); i++) {
            hs[nums[i]] = i;
        }

        for (int i = 0; i < nums.size(); i++) {
            int diff = target - nums[i];
            if (hs.count(diff) && hs[diff] != i) return {i, hs[diff]};
        }
        return {};
    }
};
