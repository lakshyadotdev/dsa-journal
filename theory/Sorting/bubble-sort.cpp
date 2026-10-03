#include <iostream>

using namespace std;

void myswap(int &a, int &b)
{
    int temp = a;
    a = b;
    b = temp;
}

void bubbleSort(int array[], int length)
{
    for (int i = 0; i < length - 1; i++)
    {
        bool swapped = false;
        for (int j = 0; j < length - 1 - i; j++)
        {
            if (array[j] > array[j + 1])
            {
                myswap(array[j], array[j + 1]);
                swapped = true;
            }
        }
        if (!swapped)
            break;
    }
}

// ============================================================
//  Step-count comparison: unoptimized (v1) vs optimized (v2)
// ============================================================

// v1: no early-termination flag
void bubbleSortV1(int a[], int n, long &cmp, long &swp)
{
    cmp = 0; swp = 0;
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - 1 - i; j++)
        {
            cmp++;
            if (a[j] > a[j + 1])
            {
                myswap(a[j], a[j + 1]);
                swp++;
            }
        }
    }
}

// v2: with early-termination flag
void bubbleSortV2(int a[], int n, long &cmp, long &swp)
{
    cmp = 0; swp = 0;
    for (int i = 0; i < n - 1; i++)
    {
        bool swapped = false;
        for (int j = 0; j < n - 1 - i; j++)
        {
            cmp++;
            if (a[j] > a[j + 1])
            {
                myswap(a[j], a[j + 1]);
                swp++;
                swapped = true;
            }
        }
        if (!swapped) break;
    }
}

void runDemo(const char *label, int src[], int n)
{
    int a1[100], a2[100];
    for (int i = 0; i < n; i++) { a1[i] = src[i]; a2[i] = src[i]; }

    long c1, s1, c2, s2;
    bubbleSortV1(a1, n, c1, s1);
    bubbleSortV2(a2, n, c2, s2);

    cout << "\n--- " << label << " (n=" << n << ") ---\n";
    cout << "v1 (no opt) : " << c1 << " comparisons, " << s1 << " swaps\n";
    cout << "v2 (w/ opt) : " << c2 << " comparisons, " << s2 << " swaps\n";
    cout << "saved       : " << (c1 - c2) << " comparisons, "
         << (s1 - s2) << " swaps\n";
}

int main()
{
    int randomArr[]   = {5, 2, 8, 1, 9, 3, 7, 4, 6};
    int sortedArr[]   = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    int reverseArr[]  = {9, 8, 7, 6, 5, 4, 3, 2, 1};

    runDemo("Random input",     randomArr,  9);
    runDemo("Already sorted",   sortedArr,  9);
    runDemo("Reverse sorted",   reverseArr, 9);

    return 0;
}
