#include <iostream>
using namespace std;
void myswap(int &a, int &b)
{
    int temp = a;
    a = b;
    b = temp;
}
void insertion_sort(int array[], int length)
{
    for (int i = 0; i <= length - 1; i++)
    {
        int j = i;
        while (j >= 0 && array[j - 1] > array[j])
        {
            myswap(array[j - 1], array[j]);
            j--;
        }
    }
}
void insertion_sort_my(int array[], int length)
{
    for (int i = 1; i < length; i++)
    {
        int key = array[i];
        int j = i - 1;
        while (j >= 0 && key < array[j])
        {
            array[j + 1] = array[j];
            j--;
        }
        array[j + 1] = key;
    }
}
int main(int argc, char const *argv[])
{
    int arr[5] = {1, 23, 45, 34, 2};
    int arr_len = sizeof(arr) / sizeof(arr[0]);
    insertion_sort_my(arr, arr_len);
    for (int i = 0; i < arr_len; i++)
    {
        cout << arr[i] << " ";
    }
    return 0;
}
