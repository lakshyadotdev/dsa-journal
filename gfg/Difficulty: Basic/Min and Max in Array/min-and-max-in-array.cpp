class Solution {
  public:
    vector<int> getMinMax(vector<int> &arr) {
        sort(arr.begin(),arr.end());
        return {*arr.begin(),*(arr.end()-1)};
    }
};