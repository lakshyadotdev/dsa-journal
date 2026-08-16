#include <iostream>
#include <vector>
using namespace std;

int binarySearch(vector<int> &arr, int &target)
{
    int start = 0;
    int end = arr.size() - 1;
    while (start <= end)
    {
        int mid = (start + end) / 2;
        if (target > arr[mid])
        {
            start = mid + 1;
        }
        else if (target < arr[mid])
        {
            end = mid - 1;
        }
        else
        {
            return mid;
        }
    }
    return -1;
}

int main(int argc, char const *argv[])
{
    vector<int> v1 = {-1, 0, 3, 4, 5, 9, 12, 134};
    int tar1 = 12;
    cout << binarySearch(v1, tar1) << endl;
    for (int i = 0; i < v1.size(); i++)
    {
        cout << v1[i] << " ";
    }
    return 0;
}
