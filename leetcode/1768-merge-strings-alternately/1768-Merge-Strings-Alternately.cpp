class Solution {
public:
    string mergeAlternately(string word1, string word2) {
        string merged = "";
        int p1 = 0;
        int p2 = 0;
        int w1 = word1.size();
        int w2 = word2.size();
        while(p1<w1 && p2<w2){
            merged += word1[p1++];
            merged +=word2[p2++];
        }
        while(p1<w1){
            merged+=word1[p1++];
        }
        while(p2<w2){
            merged+=word2[p2++];
        }
        return merged;
    }
};