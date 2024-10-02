#include <iostream>
using namespace std;
#define MOD 1000000007
long long powerMod(long long a, long long b) {
    long long rs = 1;
    a = a % MOD;
    while (b > 0) {
        if (b % 2 == 1) {
            rs = (rs * a) % MOD;
        }
        a = (a * a) % MOD;
        b /= 2;
    }
    return rs;
}
long long modInverse(long long a) {
    return powerMod(a, MOD - 2);
}
long long cMod(long long k, long long n) {
    long long numerator = 1, denominator = 1;
    for (int i = 0; i < k; i++) {
        numerator = (numerator * (n - i)) % MOD;
    }
    for (int i = 1; i <= k; i++) {
        denominator = (denominator * i) % MOD;
    }
    long long rs = ((numerator % MOD) * modInverse(denominator)) % MOD;
    return rs;
}
int main() {
    long long k, n;
    cin >> k >> n;
    long long rs = cMod(k, n);
    cout << rs << endl;
    return 0;
}