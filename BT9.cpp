#include <iostream>
#include <iomanip>
#include <cmath>
using namespace std;
int main() {
    int a, b, c;
    cin >> a >> b >> c;
    int delta = b * b - 4 * a * c;
    double x1, x2;
    if (delta < 0) {
        cout << "NO SOLUTION" << endl;
    }
    else if (delta == 0) {
        x1 = (double)-b / (2 * a);
        cout << setprecision(2) << fixed << x1 << endl;
    }
    else {
        x1 = (-b - sqrt(delta)) / (2 * a);
        x2 = (-b + sqrt(delta)) / (2 * a);
        cout << setprecision(2) << fixed << x1 << " " << x2 << endl;
    }
    return 0;
}