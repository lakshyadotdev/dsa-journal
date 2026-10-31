class Solution {
public:
    string addBinary(string a, string b) {
        int p = a.size()-1;
        int q = b.size()-1;
        size_t carry = 0;
        string result = "";
        while(p>=0 || q>=0 || carry>0){
            size_t sum = carry;
            if(p>=0){
                sum += a[p] - '0';
                p--;
            }
            if(q>=0){
                sum+= b[q] -'0';
                q--;
            }
            carry = sum/2;
            sum %= 2;
            result += (sum+'0');
        }
        reverse(result.begin(),result.end());
        return result;
    }

};