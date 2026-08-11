class Solution {
  public:
    int largest(vector<int> &arr) {
       sort(arr.begin(),arr.end());
       auto it = arr.end() - 1;
       return *it;
    }
};
