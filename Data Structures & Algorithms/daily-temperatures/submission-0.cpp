class Solution {
   public:
    vector<int> dailyTemperatures(vector<int>& temperatures) {
        int n = temperatures.size();
        vector<int> result(n, 0);
        stack<pair<int, int>> s;
        s.push({temperatures[0], 0});
        for (int i = 1; i < n; i++) {
            while (!s.empty() && temperatures[i] > s.top().first) {
                pair<int, int> stackVal = s.top();
                s.pop();
                result[stackVal.second] = i - stackVal.second;
            }
            s.push({temperatures[i], i});
        }
        return result;
    }
};
