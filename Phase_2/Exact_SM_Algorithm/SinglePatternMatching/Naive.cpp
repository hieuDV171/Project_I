#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;

vector<int> BruteForceSearch(const string &text, const string &pattern) {
    int n = text.length();
    int m = pattern.length();
    vector<int> positions;

    for (int i = 0; i <= n - m; i++) {
        bool match = true;
        for (int j = 0; j < m; j++) {
            if (text[i + j] != pattern[j]) {
                match = false;
                break;
            }
        }

        if (match) {
            positions.push_back(i);
        }
    }
    return positions;
}

int main() {
    string text, pattern;

    ifstream inputFile("input.txt");
    if (!inputFile.is_open()) {
        cerr << "Error: Could not open input file!" << endl;
        return 1;
    }
    string line;
    int i = 0;
    while (getline(inputFile, line)) {
        stringstream ss(line);
        if (getline(ss, text, ',') && getline(ss, pattern, ',')) {
            cout << setw(3) << ++i << ": ";
            vector<int> positions = BruteForceSearch(text, pattern);
            if (!positions.empty()) {
                cout << "Pattern found at index: ";
                for (int pos : positions) {
                    cout << pos << " ";
                }
                cout << endl;
            } else {
                cout << "Pattern not found!" << endl;
            }
        }
    }

    inputFile.close();
    return 0;
}