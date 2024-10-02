#include <iostream>
using namespace std;
int main() {
    string P1, P2, T;
    getline(cin, P1);
    getline(cin, P2);
    getline(cin, T);
    int l1 = P1.size();
    int l2 = P2.size();
    int l3 = T.size();
    int mark[l3];
    int A[l3];
    int k = 0;
    for (int j = l3 - 1; j >= 0; j--) {
        if (T[j] == P1[0]) {
            mark[k] = j;
            k++;
        }
    }
    int m = 0;
    bool check = true;
    for (int t = 0; t < k; t++) {
        if ((T[mark[t] + l1] >= 'A' && T[mark[t] + l1] <= 'Z') || (T[mark[t] + l1] >= 'a' && T[mark[t] + l1] <= 'z')) {
            check = false;
        }
        else {
            int i = 0;
            for (int h = mark[t]; h < mark[t] + l1; h++) {
                if (T[h] != P1[i])
                    check = false;
                i++;
            }
        }
        if (check) {
            A[m] = mark[t];
            m++;
        }
    }
    int len = l3 + (l2 - l1) * m;
    T.resize(len);
    for (int j = 0; j < m; j++) {
        for (int i = l3 + (l2 - l1) * (j + 1); i >= A[j] + l2; i--) {
            T[i] = T[i - l2 + l1];
        }
        int cnt = 0;
        for (int i = A[j]; i <= A[j] + l2 - 1; i++) {
            T[i] = P2[cnt];
            cnt++;
        }
    }
    for (int i = 0; i < len; i++) {
        cout << T[i];
    }
    return 0;
}