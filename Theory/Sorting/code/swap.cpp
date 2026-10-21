#include <iostream>
using namespace std;
void myswap(int &a, int &b)
{
    int temp = a;
    a = b;
    b = temp;
}
int main(int argc, char const *argv[])
{
    int num1 = 2;
    int num2 = 45;
    myswap(num1, num2);
    cout << "num1: " << num1 << " num2: " << num2;
    return 0;
}
