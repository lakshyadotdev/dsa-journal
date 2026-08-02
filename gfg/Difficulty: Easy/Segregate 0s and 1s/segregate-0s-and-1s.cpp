class Solution {
	public:
	void swap(int &a, int &b) {
		int temp = a;
		a = b;
		b = temp;
	}
	void segregate0and1(vector<int> &arr) {
		int n = arr.size();
		int lastZero = 0;
		for(int i = 0; i<n;i++){
		    if(arr[i]==0){
		        swap(arr[i],arr[lastZero]);
		        lastZero++;
		    }
		}
	}
};
