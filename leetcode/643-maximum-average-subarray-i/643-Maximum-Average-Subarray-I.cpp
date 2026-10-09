class Solution {
public:
    double findMaxAverage(vector<int>& nums, int k) {
        int sum = 0;
        for(int i = 0; i<k; i++){
            sum += nums[i];
        }
        int maxSum = sum;
        for(int i = k; i<nums.size(); i++){
            sum+= nums[i];
            sum-=nums[i-k];
            maxSum = max(sum,maxSum);
        }
        return (double)maxSum/k;
    }
};