#include <iostream>
using namespace std;
void swap(int &a, int &b)
{
    int temp = a;
    a = b;
    b = temp;
}
void selection_sort(int array[], int length)
{
    for (int i = 0; i < length - 1; i++)
    {
        int min_index = i;
        for (int j = i + 1; j < length; j++)
        {
            if (array[min_index] > array[j])
            {
                min_index = j;
            }
        }
        swap(array[i], array[min_index]);
    }
}
int main(int argc, char const *argv[])
{
    int arr[5] = {1, 23, 45, 34, 2};
    int arr_len = sizeof(arr) / sizeof(arr[0]);
    selection_sort(arr, arr_len);
    for (int i = 0; i < arr_len; i++)
    {
        cout << arr[i] << " ";
    }
    return 0;
}
