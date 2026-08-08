class Solution {
  public:
        int i = 1;
    void printTillN(int n) {
        if(n<1){
            return;
        }
        cout << i << " ";
        i++;
        printTillN(n-1);
    }
};