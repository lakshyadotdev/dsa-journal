class Solution {
  public:
    int ans = 1;
    int factorial(int n) {
        if(n<=1){
            return ans;
        }
        ans = ans*n;
        factorial(n-1);
    }
};