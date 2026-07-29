class Solution {
   public:
    vector<int> topKFrequent(vector<int>& nums, int k) {
        unordered_map<int, int> count;
        for (auto num : nums) {
            count[num] += 1;
        }

        vector<vector<int>> buckets(nums.size() + 1);
        for (const auto& [num, freq] : count) {
            buckets[freq].push_back(num);
        }

        vector<int> ans;
        for (int i = buckets.size() - 1; i > 0; i--) {
            for (int n : buckets[i]) {
                ans.push_back(n);
            }
            if (ans.size() == k) return ans;
        }
        return ans;
    }
};
