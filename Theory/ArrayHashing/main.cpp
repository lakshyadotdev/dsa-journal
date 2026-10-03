#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>

using namespace std;
typedef vector<int> vi;
typedef unordered_set<int> set;
typedef unordered_map<int, int> map;

void arrayArithimetic()
{

    // index arithmetic
    int n = 3;
    vi arr = {1, 2, 3, 4, 2, 3};
    printf("------%%--------\n");
    for (int i = 0; i < arr.size(); i++)
    {
        printf("%d %d\n", arr[i], arr[i % n]); // n restricts array
    }
    printf("------/--------\n");
    for (int i = 0; i < arr.size(); i++)
    {
        printf("%d %d\n", arr[i], arr[i / n]); // n slows
    }
}

int main()
{
    arrayArithimetic();
    return 0;
}