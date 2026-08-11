class Solution {
  public:
    void rotate(vector<int> &arr) {
    int k = 1;
    reverse(arr.begin(),arr.end());
    reverse(arr.begin(),arr.begin()+k-1);
    reverse(arr.begin()+k, arr.end());
    }
};