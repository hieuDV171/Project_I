#include <iostream>
#include <math.h>
using namespace std;
int main() {
    string s;
    getline(cin, s);
    bool check = true;
    size_t len = s.size();
    int hh = 0, mm = 0, ss = 0;
    if (len != 8)
        check = false;
        else {
            for (int i = 0; i < len; i++) {
                if ((i == 2 || i == 5) && s[i] != ':') {
                    check = false;
                }
                else if ((i != 2 && i != 5) && (s[i] < '0' || s[i] > '9')) {
                    check = false;
                }
            }
            for (int i = 0; i < len; i++) {
                if (i < 2) {
                    hh += (s[i] - 48) * pow(10, 1 - i);
                }
                else if (i > 5) {
                    ss += (s[i] - 48) * pow(10, 7 - i);
                }
                else if (i == 3 || i == 4) {
                    mm += (s[i] - 48) * pow(10, 4 - i);
                }
            }
            if (hh > 23 || mm > 59 || ss > 59) {
                check = false;
            }
        }
        if (check) {
            int rs = hh * 3600 + mm * 60 + ss;
            cout << rs << endl;
        }
        else
            cout << "INCORRECT";
    return 0;
}