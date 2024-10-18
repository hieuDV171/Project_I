#include <iostream>
#define MOD 1000000007
using namespace std;
long long x, y;
void Extended_Euclid(long long a, long long b) {
    if (b == 0) {
        x = 1;
        y = 0;
    } else {
        Extended_Euclid(b, a % b);
        long long temp = x;
        x = y;
        y = temp - (a / b) * y;
    }
}
long long Inverse(long long a) {
    Extended_Euclid(a, MOD);
    return (x % MOD + MOD) % MOD;
}
long long cMod(long long k, long long n) {
    long long TS = 1, MS = 1;
    for (int i = 0; i < k; i++) {
        TS = (TS * (n - i)) % MOD;
    }
    for (int i = 1; i <= k; i++) {
        MS = (MS * i) % MOD;
    }
    long long rs = ((TS % MOD) * (Inverse(MS) % MOD)) % MOD;
    return rs;
}
int main() {
    long long k, n;
    cin >> k >> n;
    long long rs = cMod(k, n);
    cout << rs << endl;
    return 0;
}