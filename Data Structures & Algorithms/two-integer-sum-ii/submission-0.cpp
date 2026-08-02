class Solution {
   public:
    vector<int> twoSum(vector<int>& numbers, int target) {
        unordered_map<int, int> mapping;
        for (int i = 0; i < numbers.size(); i++) {
            int diff = target - numbers[i];
            if (mapping.find(diff) != mapping.end()) {
                return {mapping[diff] + 1, i + 1};
            }
            mapping[numbers[i]] = i;
        }
        return {};
    }
};
