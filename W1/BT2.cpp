#include <iostream>
#define N 10001
using namespace std;
int n, a[10000];
string S;
int A[N];
void readData() {
    cin >> n;
    for (int i = 1; i <= n; i++) {
        cin >> A[i];
    }
}
int MaxMinSum(string x) {
    int max = A[1], min = A[1], sum = A[1];
    for (int i = 2; i <= n; i++) {
        if (A[i] > max)
            max = A[i];
        if (A[i] < min)
            min = A[i];
        sum += A[i];
    }
    if (x == "Max")
        return max;
    else if (x == "Min")
        return min;
        else
            return sum;
}
int maxseg(int d, int e) {
    int max = A[d];
    for (int k = d + 1; k <= e; k++) {
        if (A[k] > max)
            max = A[k];
    }
    return max;
}
int main(){
    readData();
    cin >> S;
    int cnt = 0;
    while (S.compare("***") != 0) {
        cin >> S;
        if (S.compare("find-max") == 0) {
            a[cnt] = MaxMinSum("Max");
            cnt++;
        }
        else if (S.compare("find-min") == 0) {
            a[cnt] = MaxMinSum("Min");
            cnt++;
        }
        else if (S.compare("find-max-segment") == 0) {
            int i, j;
            cin >> i >> j;
            a[cnt] = maxseg(i, j);
            cnt++;
        }
        else if (S.compare("sum") == 0) {
            a[cnt] = MaxMinSum("Sum");
            cnt++;
        }
    }
    for (int i = 0; i < cnt; i++) {
        cout << a[i] << endl;
    }
    return 0;
}