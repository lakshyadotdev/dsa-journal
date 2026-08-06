class Solution {
  public:
    bool isPrime(int n) {
       int factors = 1;
       for(int i = 1; i<n; i++){
           if(n%i==0){
               factors++;
           }
       }
       return factors == 2;
    }
};
