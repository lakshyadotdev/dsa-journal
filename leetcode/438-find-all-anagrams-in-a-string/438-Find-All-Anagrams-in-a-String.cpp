class Solution {
public:
    vector<int> findAnagrams(string s, string p) {
        int k = p.size();
        unordered_map<char, int> p_map;
        unordered_map<char, int> s_map;
        vector<int> ans = {};
        if (k > s.size())
            return ans;
        for (int i = 0; i < k; i++) {
            p_map[p[i]] += 1;
            s_map[s[i]] += 1;
        }
        if (s_map == p_map) {
            ans.push_back(0);
        }
        for (int i = k; i < s.size() ; i++) {
            s_map[s[i]] += 1;
            char remove_char = s[i - k];
            s_map[remove_char] -= 1;
            if(s_map[remove_char] == 0){
                s_map.erase(remove_char);
            }
            if (s_map == p_map) {
                ans.push_back(i - k + 1);
            }
        }
        return ans;
    }
};