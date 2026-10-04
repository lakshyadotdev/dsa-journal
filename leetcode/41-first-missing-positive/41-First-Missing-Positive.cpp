class Solution {
public:
    int firstMissingPositive(vector<int>& nums) {
        unordered_set<int> m;
        if (nums.empty()) return 1;
        long long n = nums[0];
        for (int i = 0; i < nums.size(); i++) {
            if (nums[i] >= 0) {
                m.insert(nums[i]);
                if (nums[i] > n) {
                    n = nums[i];
                }
            }
        }
        for (long long i = 1; i < n + 2; i++) {
            if (not m.contains(i)) {
                return i;
            }
        }
        return 1;
    }
};