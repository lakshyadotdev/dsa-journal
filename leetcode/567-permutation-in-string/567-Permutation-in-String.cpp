class Solution {
public:
    bool checkInclusion(string s1, string s2) {
        vector<int> arr1(26, 0);
        vector<int> arr2(26, 0);
        size_t n1 = s1.size();
        size_t n2 = s2.size();
        if (n1 > n2)
            return false;
        for (int i = 0; i < n1; i++) {
            arr1[s1[i] - 'a']++;
            arr2[s2[i] - 'a']++;
        }
        if (arr1 == arr2) {
            return true;
        }
        for (int i = n1; i < n2; i++) {
            arr2[s2[i] - 'a']++;
            arr2[s2[i - n1] - 'a']--;
            if (arr1 == arr2) {
                return true;
            }
        }
        return false;
    }
};