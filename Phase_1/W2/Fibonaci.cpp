#include <iostream>
using namespace std;
int n;
int A[22];
int Fib(int x) {
    if (x == 0){
        A[x] = 0;
    }
    else if (x == 1) {
        A[x] = 1;
    }
    else if (A[x] < 0) {
        A[x] = Fib(x - 1) + Fib(x - 2);
    }
    return A[x];
}
int main() {
    cin >> n;
    for (int i = 0; i <= n; i++) {
        A[i] = -1;
    }
    int rs = Fib(n - 1);
    cout << rs << endl;
    return 0;
}