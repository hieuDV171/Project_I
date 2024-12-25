#include <iostream>
using namespace std;
int main() {
    string s;
    int Q = 0;
    while (getline(cin, s)) {
        s += '\n';
        int l = s.size();
        for (int i = 0; i < l; i++) {
            if ((s[i + 1] == ' ' || s[i + 1] == '\t' || s[i + 1] == '\n') && (s[i] != ' ' && s[i] != '\t' && s[i] != '\n')) {
                Q++;
            }
        }
    }
    cout << Q;
    return 0;
}