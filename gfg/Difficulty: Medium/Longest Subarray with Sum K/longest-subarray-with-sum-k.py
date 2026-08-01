from collections import defaultdict

class Solution:
    def longestSubarray(self, nums, k):
        mpp = defaultdict(int)
        mpp[0] = -1

        cur = 0
        mx = 0

        for i in range(len(nums)):
            cur += nums[i]
            if cur not in mpp:
                mpp[cur] = i
            if cur-k in mpp:
                mx = max(mx, i-mpp[cur-k])
        return mx

