class Solution {
public:
    int binarySearch(const vector<int>& arr, int high, int target) {
        int low = 0;
        int mid;
        int ans = high + 1;
        while (low <= high) {
            mid = low + (high - low) / 2;
            if (arr[mid] > target) {
                ans = mid;
                high = mid - 1;
            } else {
                low = mid + 1;
            }
        }
        return ans;
    }

    int getNum(const vector<int>& nums1, const vector<int>& nums2, int target) {
        int p1 = (int)nums1.size() - 1;
        int p2 = (int)nums2.size() - 1;

        while (p1 >= 0 && p2 >= 0) {
            if (p1 + p2 + 1 == target) {
                return max(nums1[p1], nums2[p2]);
            }

            if (nums1[p1] == nums2[p2]) {
                p1--;
                continue;
            }

            if (nums1[p1] > nums2[p2]) {
                int i = binarySearch(nums1, p1, nums2[p2]);
                int merged_idx = i + (p2 + 1);
                if (target >= merged_idx) {
                    return nums1[target - (p2 + 1)];
                }
                p1 = i - 1;
            } else {
                int i = binarySearch(nums2, p2, nums1[p1]);
                int merged_idx = i + (p1 + 1);
                if (target >= merged_idx) {
                    return nums2[target - (p1 + 1)];
                }
                p2 = i - 1;
            }
        }

        if (p1 < 0)
            return nums2[target];
        return nums1[target];
    }

    double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2) {
        int total = nums1.size() + nums2.size();
        if (total % 2 == 1) {
            return getNum(nums1, nums2, total / 2);
        }
        double left = getNum(nums1, nums2, (total / 2) - 1);
        double right = getNum(nums1, nums2, total / 2);
        return (left + right) / 2.0;
    }
};