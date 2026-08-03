class Solution {
   public:
    int maxArea(vector<int>& heights) {
        int l = 0, r = heights.size() - 1, maxVal = 0;
        while (l < r) {
            int val = (r - l) * min(heights[l], heights[r]);
            maxVal = max(maxVal, val);
            if (heights[l] < heights[r])
                l++;
            else
                r--;
        }
        return maxVal;
    }
};
