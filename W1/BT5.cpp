#include <iostream>
using namespace std;
int main() {
    string a, b;
    while (getline(cin, b)) {
        a += b + '\n';
    }
    int c = a.size();
    for (int i = 0; i < c; i++) {
        a[i] = toupper(a[i]);
    }
    cout << a;
    return 0;
}