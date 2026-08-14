class Solution {
public:
    bool check(vector<int>& nums) {
        vector<int> orgNums = nums;
        for (int i = 0; i < nums.size(); i++) {
            sort(nums.begin(), nums.end());
            reverse(nums.begin(), nums.end());
            reverse(nums.begin(), nums.begin() + i);
            reverse(nums.begin() + i, nums.end());
            if (nums == orgNums) {
                return true;
            }
        }
        return false;
    }
};