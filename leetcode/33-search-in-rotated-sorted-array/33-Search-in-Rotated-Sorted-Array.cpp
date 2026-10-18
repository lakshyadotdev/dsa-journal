class Solution {
public:
    int search(vector<int>& nums, int target) {
        int low = 0;
        int high = nums.size() - 1;
        int n = nums.size();
        while (low < high) {
            int mid = low + (high - low) / 2;
            if (nums[mid] > nums[high]) {
                low = mid + 1;
            } else {
                high = mid;
            }
        }
        int k = low;
        int start = 0;
        int end = n - 1;
        while (start <= end) {
            int tempmid = start + (end - start) / 2;
            int mid = (tempmid + k) % n;
            if (nums[mid] == target) {
                return mid;
            } else if (nums[mid] < target) {
                start = tempmid + 1;
            } else {
                end = tempmid - 1;
            }
        }
        return -1;
    }
};