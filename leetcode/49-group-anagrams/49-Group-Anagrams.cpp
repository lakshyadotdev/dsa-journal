class Solution {
public:
    vector<vector<string>> groupAnagrams(vector<string>& strs) {
        unordered_map<string, vector<string>> s;
        for (int i = 0; i < strs.size(); i++) {
            string sortedStr = strs[i];
            sort(sortedStr.begin(),sortedStr.end());
            s[sortedStr].emplace_back(strs[i]);
        }
        vector<vector<string>> result;
        for(auto&[_,v]:s) result.emplace_back(v);
        return result;
    }
};