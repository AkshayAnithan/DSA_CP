#define ll long long
class Solution {
   public:
    int minEatingSpeed(vector<int>& piles, int h) {
        int l = 1, r = *max_element(piles.begin(), piles.end());
        int ans = r;
        while (l <= r) {
            int speed = l + (r - l) / 2;
            ll hoursTaken = 0;
            for (int& pile : piles) {
                hoursTaken += (pile + speed - 1) / speed;
            }
            if (hoursTaken > h) {
                l = speed + 1;
            } else {
                r = speed - 1;
                ans = min(ans, speed);
            }
        }
        return ans;
    }
};
