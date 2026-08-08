class Solution {
  public:
    void reverseArray(vector<int> &arr) {
        int p1 = 0;
        int p2 = arr.size()-1;
        while(p1<p2){
            swap(arr[p1],arr[p2]);
            p1++;
            p2--;
        }
    }
};