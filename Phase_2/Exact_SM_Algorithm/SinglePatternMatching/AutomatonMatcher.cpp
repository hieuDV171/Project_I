#include <chrono> // Để đo hiệu năng
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

unordered_map<int, unordered_map<char, int>> BuildDFA(const string &pattern) {
    unordered_map<int, unordered_map<char, int>> dfa;
    int m = pattern.length();

    dfa[0][pattern[0]] = 1;

    int lps = 0;
    for (int i = 1; i < m; i++) {
        for (auto &[ch, state] : dfa[lps]) {
            dfa[i][ch] = state;
        }

        dfa[i][pattern[i]] = i + 1;

        lps = dfa[lps][pattern[i]];
    }

    for (auto &[ch, state] : dfa[lps]) {
        dfa[m][ch] = state;
    }

    return dfa;
}

void SaveDFAToFile(const unordered_map<int, unordered_map<char, int>> &dfa, const string &filename) {
    ofstream outFile(filename, ios::binary);
    if (!outFile) {
        cerr << "Error: Could not open file for saving DFA!" << endl;
        return;
    }

    size_t dfaSize = dfa.size();
    outFile.write(reinterpret_cast<char *>(&dfaSize), sizeof(dfaSize));

    for (const auto &stateEntry : dfa) {
        int state = stateEntry.first;
        outFile.write(reinterpret_cast<char *>(&state), sizeof(state));

        size_t transitionsSize = stateEntry.second.size();
        outFile.write(reinterpret_cast<char *>(&transitionsSize), sizeof(transitionsSize));

        for (const auto &transition : stateEntry.second) {
            outFile.write(reinterpret_cast<const char *>(&transition.first), sizeof(transition.first));   // character
            outFile.write(reinterpret_cast<const char *>(&transition.second), sizeof(transition.second)); // next state
        }
    }

    outFile.close();
    cout << "DFA has been saved to file!" << endl;
}

unordered_map<int, unordered_map<char, int>> LoadDFAFromFile(const string &filename) {
    unordered_map<int, unordered_map<char, int>> dfa;

    ifstream inFile(filename, ios::binary);
    if (!inFile) {
        cerr << "Error: Could not open file for reading DFA!" << endl;
        return dfa;
    }

    size_t dfaSize;
    inFile.read(reinterpret_cast<char *>(&dfaSize), sizeof(dfaSize));

    for (size_t i = 0; i < dfaSize; ++i) {
        int state;
        inFile.read(reinterpret_cast<char *>(&state), sizeof(state));

        size_t transitionsSize;
        inFile.read(reinterpret_cast<char *>(&transitionsSize), sizeof(transitionsSize));

        unordered_map<char, int> transitions;
        for (size_t j = 0; j < transitionsSize; ++j) {
            char ch;
            int nextState;
            inFile.read(reinterpret_cast<char *>(&ch), sizeof(ch));
            inFile.read(reinterpret_cast<char *>(&nextState), sizeof(nextState));
            transitions[ch] = nextState;
        }

        dfa[state] = transitions;
    }

    inFile.close();
    cout << "DFA has been loaded from file!" << endl;

    return dfa;
}

// Hàm Automaton Matcher
vector<int> AutomatonMatcher(const string &text, const string &pattern) {
    unordered_map<int, unordered_map<char, int>> dfa = BuildDFA(pattern);
    vector<int> positions;

    int n = text.length();
    int m = pattern.length();
    int state = 0;

    for (int i = 0; i < n; i++) {
        if (dfa[state].count(text[i])) {
            state = dfa[state][text[i]];
        } else {
            state = 0;
        }

        if (state == m) {
            positions.push_back(i - m + 1);
        }
    }

    return positions;
}

int main() {
    string text, pattern;
    unordered_map<int, unordered_map<char, int>> dfa;
    bool dfaLoaded = false;

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

            if (!dfaLoaded) {
                // Nếu chưa có DFA, tạo mới và lưu nó vào tệp
                dfa = BuildDFA(pattern);
                SaveDFAToFile(dfa, "dfa.bin");
                dfaLoaded = true;
            } else {
                // Nếu DFA đã được lưu trước đó, chỉ cần tải lại
                dfa = LoadDFAFromFile("dfa.bin");
            }

            // Tìm kiếm mẫu trong văn bản
            vector<int> positions = AutomatonMatcher(text, pattern);
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