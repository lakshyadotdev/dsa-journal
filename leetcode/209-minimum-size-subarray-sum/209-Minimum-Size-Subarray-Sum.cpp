class Solution {
public:
    int minSubArrayLen(int target, vector<int>& nums) {
        int minLen = nums.size() + 1;
        int right = 1;
        int left = 0;
        int sum = nums[left];
        while (left <= right) {
            if (sum < target && right < nums.size()) {
                sum += nums[right];
                right++;
            } else if (left < nums.size() && sum >= target) {
                minLen = min(minLen, right - left);
                sum -= nums[left];
                left++;
            }
            else{
                break;
            }
        }
        return minLen>nums.size()?0:minLen;
    }
};