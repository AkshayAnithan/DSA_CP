class Solution {
   public:
    int maxProfit(vector<int>& prices) {
        int minPrice = prices[0];
        int maxProfits = 0;

        for (int& price : prices) {
            maxProfits = max(maxProfits, price - minPrice);
            minPrice = min(price, minPrice);
        }
        return maxProfits;
    }
};
