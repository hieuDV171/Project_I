#include <iostream>
using namespace std;
int main() {
    string S;
    int Q = 0;
    while (cin >> S) {
        Q++;
        cout << S << endl;
    }
    cout << Q;
    return 0;
}