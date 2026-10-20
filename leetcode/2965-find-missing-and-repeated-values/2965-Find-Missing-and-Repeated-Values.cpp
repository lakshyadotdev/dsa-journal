class Solution {
public:
    vector<int> findMissingAndRepeatedValues(vector<vector<int>>& grid) {
        vector<int> ans(2, 0);
        vector<int> m(grid.size() * grid.size() + 1, 0);
        for (int i = 0; i < grid.size(); i++) {
            for (int j = 0; j < grid.size(); j++) {
                m[grid[i][j]]++;
            }
        }
        for(int i = 1; i<=grid.size()*grid.size(); i++){
            if(m[i]==2){
                ans[0] = i;
            }
            else if(m[i]==0){
                ans[1] = i;
            }
        }
        return ans;
    }
};