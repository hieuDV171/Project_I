#include <iostream>
using namespace std;
int markR[9][10];
int markC[9][10];
int markS[3][3][10];
int x[9][9];
int cnt = 0;
void solution() {
    for (int r = 0; r < 9; r++) {
        for (int c = 0; c < 9; c++) {
            cout << x[r][c] << " ";
        }
        cout << endl;
    }
}
int check(int v, int r, int c) {
    if (markR[r][v] == 1 || markC[c][v] == 1 || markS[r / 3][c / 3][v] == 1) {
        return 0;
    }
    return 1;
}
void Try(int r, int c) {
    if (x[r][c] == 0) {
        for (int v = 1; v <= 9; v++) {
            if (check(v, r, c)) {
                x[r][c] = v;
                markR[r][v] = 1;
                markC[c][v] = 1;
                markS[r / 3][c / 3][v] = 1;
                if (r == 8 && c == 8) {
                    cnt++;
                    solution();
                } else if (c < 8) {
                    Try(r, c + 1);
                } else {
                    Try(r + 1, 0);
                }
                x[r][c] = 0;
                markR[r][v] = 0;
                markC[c][v] = 0;
                markS[r / 3][c / 3][v] = 0;
            }
        }
    } else {
        if (r == 8 && c == 8) {
            cnt++;
            solution();
        } else if (c < 8) {
            Try(r, c + 1);
        } else {
            Try(r + 1, 0);
        }
    }
}
int main() {
    for (int v = 0; v < 10; v++) {
        for (int r = 0; r < 9; r++) {
            markR[r][v] = 0;
        }
        for (int c = 0; c < 9; c++) {
            markC[c][v] = 0;
        }
        for (int I = 0; I < 3; I++) {
            for (int J = 0; J < 3; J++) {
                markS[I][J][v] = 0;
            }
        }
    }
    for (int r = 0; r < 9; r++) {
        for (int c = 0; c < 9; c++) {
            cin >> x[r][c];
            if (x[r][c] > 0) {
                markR[r][x[r][c]] = 1;
                markC[c][x[r][c]] = 1;
                markS[r / 3][c / 3][x[r][c]] = 1;
            }
        }
    }
    cout << endl;
    Try(0, 0);
    cout << cnt << endl;
    return 0;
}