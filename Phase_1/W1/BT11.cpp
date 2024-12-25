#include <iostream>
using namespace std;
int main() {
    size_t n, a;
    cin >> n;
    for (size_t i = 1; i <= n; i++) {
        a = i * i;
        cout << i << " " << a << endl;
    }
    return 0;
}