class Solution {
public:
    int minEatingSpeed(vector<int>& piles, int h) {
        int high = *max_element(piles.begin(),piles.end());
        int low = 1;
        int ans = high;
        while(low<=high){
            int mid = low + (high-low)/2;
            long long t = eat(piles,mid);
            if(t>h){
                low = mid+1;
            }
            else if(t<=h){
                ans = mid;
                high = mid-1;
            }
        }
        return ans;
    }
    long long eat(vector<int>& piles,int k){
        long long totalHours = 0;
        for(int p:piles){
            totalHours += (p+k-1)/k;
        }
        return totalHours;
    }
};