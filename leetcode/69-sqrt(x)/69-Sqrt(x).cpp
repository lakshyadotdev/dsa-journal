class Solution {
public:
    int mySqrt(int x) {
        int low = 0;
        int high = x;
        int mid;
        while (low <= high) {
            mid = low + (high - low) / 2;
            long long sq = static_cast<long long>(mid)*mid;
            if(low==high){
                return sq<=x?low:low-1;
            };
            if (sq > x ) {
                high = mid - 1;
            } else if (sq < x) {
                low = mid + 1;
            } else if (sq == x) {
                return mid;
            }
        }
        return mid < 1 ? 0 : mid - 1;
    }
};