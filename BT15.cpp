#include <iostream>
using namespace std;
int n;
int x[10001];
int check(int v, int k) {
    for (int i = 1; i <= k; i++) {
        if (x[i] == v)
            return 0;
    }
    return 1;
}
void solution() {
    for (int i = 1; i <= n; i++) {
        cout << x[i] << " ";
    }
    cout << endl;
}
void Try(int k) {
    for (int v = 1; v <= n; v++) {
        if (check(v, k)) {
            x[k] = v;
            if (k == n) {
                solution();
            }
            else {
                Try(k + 1);
            }
            x[k] = 0; //backtrack
        }
    }
}
int main() {
    cin >> n;
    for (int i = 1; i <= n; i++) {
        x[i] = 0;
    }
    Try(1);
    return 0;
}