#include <iostream>
using namespace std;
int main() {
    string P1, P2, T;
    getline(cin, P1);
    getline(cin, P2);
    getline(cin, T);
    int a = T.find(P1);
    while (a != string::npos) {
        T.replace(a, P1.size(), P2);
        a = T.find(P1, a + P2.size());
    }
    cout << T << endl;
    return 0;
}