class Solution {
   public:
    vector<int> productExceptSelf(vector<int>& nums) {
        int n = nums.size(), zeroCount = 0;
        int product = 1;
        for (auto& num : nums) {
            if (num != 0)
                product *= num;
            else
                zeroCount++;
        }
        if (zeroCount > 1) {
            return vector<int>(n, 0);
        }
        vector<int> res;
        for (auto& num : nums) {
            if (zeroCount > 0) {
                res.push_back(num == 0 ? product : 0);
            } else {
                res.push_back(product / num);
            }
        }
        return res;
    }
};
