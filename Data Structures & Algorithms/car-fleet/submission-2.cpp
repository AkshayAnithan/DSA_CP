class Solution {
   public:
    int carFleet(int target, vector<int>& position, vector<int>& speed) {
        int n = position.size();
        stack<double> time;
        vector<pair<int, int>> cars;

        for (int i = 0; i < n; i++) {
            cars.push_back({position[i], speed[i]});
        }

        sort(cars.rbegin(), cars.rend());
        vector<double> result;
        for (auto& c : cars) {
            result.push_back((double)(target - c.first) / c.second);
            if (result.size() >= 2 && result.back() <= result[result.size() - 2]) {
                result.pop_back();
            }
        }
        return result.size();
    }
};
