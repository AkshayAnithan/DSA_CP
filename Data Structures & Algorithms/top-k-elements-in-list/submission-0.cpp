class Solution {
   public:
    vector<int> topKFrequent(vector<int>& nums, int k) {
        unordered_map<int, int> count;
        for (auto num : nums) {
            count[num] += 1;
        }
        vector<pair<int, int>> freqCount(count.begin(), count.end());
        sort(freqCount.begin(), freqCount.end(),
             [](const pair<int, int>& a, const pair<int, int>& b) { return a.second > b.second; });

        vector<int> ans;
        for (int i = 0; i < k; i++) {
            ans.push_back(freqCount[i].first);
        }
        return ans;
    }
};
