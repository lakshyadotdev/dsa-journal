class Solution {
public:
    int lengthOfLongestSubstring(string s) {
        unordered_set<char> win;
        int left = 0;
        int right = 0;
        int maxSize = 0;
        while(right<s.size()){
            while(win.contains(s[right])){
                win.erase(s[left]);
                left++;
            }
            maxSize = max(maxSize,right-left+1);
            win.insert(s[right]);
            right++;
        }
        return maxSize;
    }
};