#define ll long long
class Solution {
   public:
    int minEatingSpeed(vector<int>& piles, int h) {
        int maxPile = *max_element(piles.begin(), piles.end());
        int left = 1, right = maxPile;
        int ans;
        while (left <= right) {
            int speed = left + (right - left) / 2;
            ll hours = 0;
            for (int pile : piles) {
                hours += (pile + speed - 1) / speed;
            }
            if (hours > h) {
                left = speed + 1;
            } else {
                ans = speed;
                right = speed - 1;
            }
        }
        return ans;
    }
};
