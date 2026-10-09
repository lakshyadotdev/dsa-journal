class Solution {
public:
    int longestConsecutive(vector<int>& nums) {
        unordered_set<int> s;
        int maxCount = 0;
        s.insert(nums.begin(), nums.end());
        for(int num: s){
            int j = 0;
            if(!s.contains(num-1)){
                while(s.contains(num+j)){
                    j++;
                }
            }
            maxCount = max(maxCount,j);
        }
        return maxCount;
    }
};