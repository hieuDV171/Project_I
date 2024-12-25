#include <iostream>
#define N 100001
using namespace std;
int main() {
    size_t n, O = 0, E = 0;
    int A[N];
    cin >> n;
    for (size_t i = 0; i < n; i++) {
        cin >> A[i];
        if (A[i] % 2 == 0)
            E++;
        else
            O++;
    }
    cout << O << " " << E << endl;
    return 0;
}