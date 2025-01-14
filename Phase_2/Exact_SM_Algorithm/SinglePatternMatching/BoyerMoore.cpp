// Hiệu quả cao trên các văn bản lớn, đặc biệt nếu mẫu tương đối dài.
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

// Hàm tạo bảng "Bad Character"
unordered_map<char, int> BuildBadCharacterTable(const string &pattern) {
    unordered_map<char, int> badCharTable;
    int m = pattern.length();
    for (int i = 0; i < m; i++) {
        badCharTable[pattern[i]] = i;
    }
    return badCharTable;
}

// Hàm tạo bảng "Good Suffix"
vector<int> BuildGoodSuffixTable(const string &pattern) {
    int m = pattern.length();
    vector<int> goodSuffixTable(m + 1, m);

    vector<int> border(m + 1, 0); // Lưu các đường viền (border)
    int j = 0;
    for (int i = m - 1; i >= 0; i--) {
        while (j > 0 && pattern[i] != pattern[m - 1 - j]) {
            j = border[j];
        }
        if (pattern[i] == pattern[m - 1 - j]) {
            j++;
        }
        border[i] = j;
    }

    for (int i = 0; i < m; i++) {
        int shift = m - border[i];
        goodSuffixTable[shift] = shift;
    }
    return goodSuffixTable;
}

// Hàm Boyer-Moore
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

        // So sánh từ phải sang trái
        while (j >= 0 && text[i + j] == pattern[j]) {
            j--;
        }

        if (j < 0) {
            positions.push_back(i);
            i += (i + m < n) ? goodSuffixTable[1] : 1;
        } else {
            int badCharShift = (badCharTable.find(text[i + j]) != badCharTable.end())
                                   ? j - badCharTable[text[i + j]]
                                   : j + 1;
            int goodSuffixShift = (j < m - 1) ? goodSuffixTable[m - j - 1] : 1;
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