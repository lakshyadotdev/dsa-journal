class Solution {
public:
    vector<int> majorityElement(vector<int>& nums) {
        unordered_map<int,int> m;
        int n = nums.size();
        for(int i = 0; i<n; i++){
            m[nums[i]]++;
        }
        vector<int> v;
        for(const auto&[num,freq]:m){
            if(freq>n/3){
                v.push_back(num);
            }
        }
        return v;
    }
};