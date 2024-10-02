#include <iostream>
#define M 1000000007
using namespace std;
long long qPowMod(long long a, long long b) {
    long long rs = 1;
    a = a % M;
    while (b > 0) {
        if (b % 2 == 1) {
            rs = (rs * a) % M;
        }
        a = (a * a) % M;
        b /= 2;
    }
    return rs;
}
long long modInverse(long long a) {
    return qPowMod(a, M - 2);
}
long long cMod(long long k, long long n) {
    long long TS = 1, MS = 1;
    for (int i = 0; i < k; i++) {
        TS = (TS * (n - i)) % M;
    }
    for (int i = 1; i <= k; i++) {
        MS = (MS * i) % M;
    }
    long long rs = (TS % M * modInverse(MS)) % M;
    return rs;
}
int main() {
    long long k, n;
    cin >> k >> n;
    long long rs = cMod(k, n);
    cout << rs << endl;
    return 0;
}