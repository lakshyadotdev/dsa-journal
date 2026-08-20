class Solution {
public:
    void reverseString(vector<char>& s) {
        int low = 0;
        int n = s.size();
        int high = n-1;
        if(n<2) return;
        for(int i = 0; i<n/2; i++){
            swap(s[low],s[high]);
            low++;
            high--;
        }
    }
};