#include <bits/stdc++.h>
using namespace std;

int main() {
    int n;
    cin >> n;

    int i,j,val = 1;
    for(i =1 ; i<=n; i++){
        for(j=1;j<=i;j++){
            cout << val++ << " ";
        }
        cout << endl; 
    }

    return 0;
}