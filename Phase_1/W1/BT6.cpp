#include <iostream>
#define N 100001
using namespace std;
int main() {
    int n, j, k, Q = 0;
    cin >> n >> k;
    int A[N];
    int sum = 0;
    for (int i = 1; i <= n; i++) {
        cin >> A[i];
    }
    for (int i = 1; i <= k; i++) {
        sum += A[i];
    }
    if (sum % 2 == 0)
        Q++;
    for (int i = 1; i < n - k + 1; i++) {
        sum = sum - A[i] + A[i + k];
        if (sum % 2 == 0)
            Q++;
    }
    cout << Q;
    return 0;
}