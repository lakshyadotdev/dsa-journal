class Solution {
public:
    int scoreOfString(string s) {
        int sum = 0;
        for(int i = 0; i<s.size()-1; i++){
            sum += abs(s[i],s[i+1]);
        }
        return sum;
    }
    int abs(char c1, char c2) { return c1 - c2 > 0 ? c1 - c2 : c2 - c1; }
};