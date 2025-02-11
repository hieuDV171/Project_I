// Highly efficient on large texts, especially if the pattern is relatively long.
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

// Function to create "Bad Character" table
unordered_map<char, int> BuildBadCharacterTable(const string &pattern) {
    unordered_map<char, int> badCharTable;
    int m = pattern.length();
    for (int i = 0; i < m; i++) {
        badCharTable[pattern[i]] = i;
    }
    return badCharTable;
}

vector<int> BuildGoodSuffixTable(const string &pattern) {
    int m = pattern.length();
    vector<int> shift(m + 1, 0);
    vector<int> bpos(m + 1, 0); // border position

    int i = m, j = m + 1;
    bpos[i] = j;

    while (i > 0) {
        while (j <= m && pattern[i - 1] != pattern[j - 1]) {
            if (shift[j] == 0) {
                shift[j] = j - i;
            }
            j = bpos[j];
        }
        i--;
        j--;
        bpos[i] = j;
    }

    j = bpos[0];
    for (i = 0; i <= m; i++) {
        if (shift[i] == 0) {
            shift[i] = j;
        }
        if (i == j) {
            j = bpos[j];
        }
    }

    return shift;
}

// Boyer-Moore function
vector<int> BoyerMooreSearch(const string &text, const string &pattern) {
    int n = text.length();
    int m = pattern.length();
    vector<int> positions;

    if (m > n) {
        return positions;
    }

    auto badCharTable = BuildBadCharacterTable(pattern);
    auto goodSuffixTable = BuildGoodSuffixTable(pattern);

    int i = 0;
    while (i <= n - m) {
        int j = m - 1;

        while (j >= 0 && text[i + j] == pattern[j]) {
            j--;
        }

        if (j < 0) {
            positions.push_back(i);
            i += (i + m < n) ? goodSuffixTable[0] : 1;
        } else {
            int badCharShift = (badCharTable.find(text[i + j]) != badCharTable.end())
                                   ? j - badCharTable[text[i + j]]
                                   : j + 1;
            int goodSuffixShift = (j < m - 1) ? goodSuffixTable[j + 1] : 1;
            i += max(badCharShift, goodSuffixShift);
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
            vector<int> positions = BoyerMooreSearch(text, pattern);
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