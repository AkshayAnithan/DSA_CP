#define ll long long
class Solution {
   public:
    int minEatingSpeed(vector<int>& piles, int h) {
        auto top = max_element(piles.begin(), piles.end());
        int l = 1, r = top[0];
        int speed = 0;
        while (l <= r) {
            double m = l + (r - l) / 2;
            ll hours = 0;
            for (double x : piles) {
                hours += ceil(x / m);
            }
            if (hours > h) {
                l = m + 1;
            } else {
                speed = m;
                r = m - 1;
            }
        }
        return speed;
    }
};
