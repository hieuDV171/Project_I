#include <iostream>
#include <iomanip>
using namespace std;
int main() {
    size_t n;
    const int M1C = 1728, M2C = 1786, M3C = 2074,
              M4C = 2612, M5C = 2919, M6C = 3015,
              M1M = M1C, M2M = M3C, M3M = M4C,
              M4M = 3111, M5M = 3457;
    double T1, T2, D;
    cin >> n;
    if (n <= 50) {
        T1 = n * M1C;
        T2 = n * M1M;
    }
    else if (n <= 100) {
        T1 = 50 * M1C + (n - 50) * M2C;
        T2 = n * M1M;
    }
    else if (n <= 200) {
        T1 = 50 * M1C + 50 * M2C + (n - 100) * M3C;
        T2 = 100 * M1M + (n - 100) * M2M;
    }
    else if (n <= 300) {
        T1 = 50 * M1C + 50 * M2C + 100 * M3C + (n - 200) * M4C;
        T2 = 100 * M1M + 100 * M2M + (n - 200) * M3M;
    }
    else if (n <= 400) {
        T1 = 50 * M1C + 50 * M2C + 100 * M3C + 100 * M4C + (n - 300) * M5C;
        T2 = 100 * M1M + 100 * M2M + (n - 200) * M3M;
    }
    else if (n <= 700) {
        T1 = 50 * M1C + 50 * M2C + 100 * M3C + 100 * M4C + 100 * M5C + (n - 400) * M6C;
        T2 = 100 * M1M + 100 * M2M + 200 * M3M + (n - 400) * M4M;
    }
    else {
        T1 = 50 * M1C + 50 * M2C + 100 * M3C + 100 * M4C + 100 * M5C + (n - 400) * M6C;
        T2 = 100 * M1M + 100 * M2M + 200 * M3M + 300 * M4M + (n - 700) * M5M;
    }
    D = (T2 - T1) * 1.1;
    cout << setprecision(2) << fixed << D << endl;
    return 0;
}