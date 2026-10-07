class Solution {
public:
    bool containsNearbyDuplicate(vector<int>& nums, int k) {
        int n = nums.size();
        unordered_set<int> s;
        for (int i = 0; i < n; i++) {
            if (i > k) {
                s.erase(nums[i - k - 1]);
            }
            if (s.contains(nums[i])) {
                return true;
            }
            s.insert(nums[i]);
        }
        return false;
    }
};