#include <iostream>
using namespace std;

int printGFG(int n){
    if(n<1){
        return 0;
    }
    cout<< "GFG ";
    printGFG(n-1);
}

int main() {
    int n;
    cin >> n;
    printGFG(n);
    return 0;
}
