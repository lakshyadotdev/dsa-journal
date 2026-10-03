class Solution {
	public:
	vector<vector<int>> getPairs(vector<int>& arr) {
		sort(arr.begin(), arr.end());
		vector<vector<int>> ans;
		int i = 0;
		int j = arr.size() - 1 ;
		while (i<j) {
			int sum = arr[i]+arr[j];
			if (sum<0) {
				i++;
			}
			else if (sum>0) {
				j--;
			}
			else {
				ans.push_back({arr[i], arr[j]});
				int leftVal = arr[i];
				int rightVal = arr[j];
				while (i < j && arr[i] == leftVal)
					i++;
				while (i < j && arr[j] == rightVal)
					j--;
			}
		}
		return ans;
	}
};
