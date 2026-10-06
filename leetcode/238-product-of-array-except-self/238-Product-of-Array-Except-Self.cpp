class Solution {
public:
    vector<int> productExceptSelf(vector<int>& nums) {
        vector<int> result(nums.size(), 1);
        int m = 1;
        for (int i = 1; i < nums.size(); i++) {
            m *= nums[i - 1];
            result[i] = m;
        }
        m = 1;
        for (int i = nums.size() - 1; i > 0; i--) {
            m *= nums[i];
            result[i - 1] *= m;
        }
        return result;
    }
};