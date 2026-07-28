class Solution {
   public:
    vector<int> twoSum(vector<int>& nums, int target) {
        unordered_map<int, int> hs;
        for (int i = 0; i < nums.size(); i++) {
            int diff = target - nums[i];
            if (hs.find(diff) != hs.end()) return {hs[diff], i};
            hs.insert({nums[i], i});
        }
        return {};
    }
};
