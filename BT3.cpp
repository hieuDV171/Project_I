#include <iostream>
using namespace std;
int main() {
    int n;
    int s, b;
    cin >> n;
    s = 100 / n;
    if (100 % n == 0)
        b = s;
        else
            b = s + 1;
    for (int i = b * n; i <= 999; i += n) {
        cout << i << " ";
    }
    return 0;
}