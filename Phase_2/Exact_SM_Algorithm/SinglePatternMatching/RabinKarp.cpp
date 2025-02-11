#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

// Rabin-Karp Algorithm
vector<int> RabinKarpSearch(const string &text, const string &pattern) {
    const int prime = INT_MAX;
    const int d = 256;
    vector<int> positions;

    int n = text.length();
    int m = pattern.length();
    int patternHash = 0;
    int textHash = 0;
    int h = 1;

    for (int i = 0; i < m - 1; i++) {
        h = (h * d) % prime; // weight of first character
    }

    for (int i = 0; i < m; i++) {
        patternHash = (d * patternHash + pattern[i]) % prime;
        textHash = (d * textHash + text[i]) % prime;
    }

    for (int i = 0; i <= n - m; i++) {
        if (patternHash == textHash) {
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

        if (i < n - m) {
            textHash = (d * (textHash - text[i] * h) + text[i + m]) % prime;

            if (textHash < 0) {
                textHash += prime;
            }
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
            vector<int> positions = RabinKarpSearch(text, pattern);
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