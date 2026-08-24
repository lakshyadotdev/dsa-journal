class Solution {
public:
    bool isPalindrome(string str, int low, int high) {
        while (low < high) {
            if (str[low] != str[high])
                return false;
            low++;
            high--;
        }
        return true;
    }
    bool validPalindrome(string s) {
        int n = s.size();
        int i = 0;
        int j = n - 1;
        while (i < j) {
            if (s[i] == s[j]) {
                i++;
                j--;
            } else {
                return isPalindrome(s, i + 1, j) || isPalindrome(s, i, j - 1);
            }
        }
        return true;
    }
};