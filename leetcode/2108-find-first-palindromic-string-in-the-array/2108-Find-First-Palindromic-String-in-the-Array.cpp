class Solution {
public:
    bool checkPal(string s){
        int p1 = 0;
        int p2 = s.size()-1;
        while(p1<p2){
            if(s[p1]!=s[p2]){
                return false;
            }
            p1++;
            p2--;
        }
        return true;
    }
    string firstPalindrome(vector<string>& words) {
        string result = "";
        for(int i = 0; i<words.size();i++){
            if(checkPal(words[i])){
                return words[i];
            }
        }
        return result;
    }
};