class Solution {
public:
    int findMin(vector<int>& nums) {
        int n = nums.size();
        int high = n - 1;
        int low = 0;
        int mid;
        if(n==1) return nums[0];
        while (low < high) {
            mid = low + (high - low) / 2;
            if(nums[mid]>nums[high]){
                low = mid+1;
            }
            else{
                high = mid;
            }
        }
        return nums[low];
    }
};