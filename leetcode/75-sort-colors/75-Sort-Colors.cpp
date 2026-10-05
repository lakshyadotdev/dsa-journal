class Solution {
public:
    void sortColors(vector<int>& nums) {
        int n = nums.size();
        vector<int> v = {0, 0, 0};
        for (int i = 0; i < n; i++) {
            v[nums[i]]++;
        }
        int index = 0;
        for (int i = 0; i < v.size(); i++) {
            while (v[i] > 0) {
                nums[index] = i;
                v[i]--;
                index++;
            }
        }
    }
};