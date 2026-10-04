class Solution {
public:
    int maxProfit(vector<int>& prices) {
        int maxProfit = 0;
        int minPrice = prices[0];
        for (int i = 0; i < prices.size(); i++) {
            int price = prices[i];
            if (price < minPrice) {
                minPrice = price;
            }
            if (maxProfit < (price - minPrice)) {
                maxProfit = price - minPrice;
            }
        }
        return maxProfit;
    }
};