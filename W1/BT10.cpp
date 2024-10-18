#include <iostream>
#include <math.h>
using namespace std;
int main() {
    string s;
    getline(cin, s);
    bool check = true;
    int y = 0, m = 0, d = 0;
    size_t len = s.size();
    if (len != 10)
        check = false;
    else {
        for (int i = 0; i < len; i++) {
            if ((i == 4 || i == 7) && s[i] != '-')
                check = false;
            else if ((i != 4 && i != 7) && (s[i] < '0' || s[i] > '9')) {
                check = false;
                cout << s[i] << endl;
            }
        }
        for (size_t i = 0; i < len; i++) {
            if (i < 4) {
                y += (s[i] - 48) * pow(10, (3 - i));
            }
            else if (i == 5 || i == 6) {
                m += (s[i] - 48) * pow(10, 6 - i);
            }
            else if (i == 8 || i == 9) {
                d += (s[i] - 48) * pow(10, 9 - i);
            }
        }
        if ((m < 1 || m > 12) || (d < 1 || d > 31))
            check = false;
    }
    if (check) {
        cout << y << " " << m << " " << d << endl;
    }
    else
        cout << "INCORRECT" << endl;
    return 0;
}