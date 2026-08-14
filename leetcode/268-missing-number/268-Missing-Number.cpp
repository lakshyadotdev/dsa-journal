class Solution {
public:
    int missingNumber(vector<int>& nums) {
        int n = nums.size();
        int sum = n*(n+1)*0.5;
        return sum - accumulate(nums.begin(),nums.end(),0);
    }
};