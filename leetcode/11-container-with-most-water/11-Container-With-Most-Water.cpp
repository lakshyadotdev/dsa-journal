class Solution {
public:
    int maxArea(vector<int>& height) {
        int left = 0;
        int right = height.size() - 1;
        int maxArea = min(height[left],height[right])*(right-left);
        while(left<right){
            if(height[left]<height[right]){
                left++;
            }
            else if(height[left]>=height[right]){
                right--;
            }
            maxArea = max(maxArea,min(height[left],height[right])*abs(right-left));
        }
        return maxArea;
    }
};