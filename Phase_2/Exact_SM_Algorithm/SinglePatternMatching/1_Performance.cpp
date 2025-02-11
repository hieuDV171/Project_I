#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

using namespace std;
using namespace chrono;

struct TrieNode {
    unordered_map<char, TrieNode *> children;
    bool isEndOfWord = false;
};

vector<int> BruteForceSearch(const string &text, const string &pattern) {
    vector<int> positions;
    int n = text.length();
    int m = pattern.length();
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

vector<int> computeLPS(const string &pattern) {
    int m = pattern.length();
    vector<int> lps(m, 0);
    int len = 0;
    int i = 1;

    while (i < m) {
        if (pattern[i] == pattern[len]) {
            len++;
            lps[i] = len;
            i++;
        } else {
            if (len != 0) {
                len = lps[len - 1];
            } else {
                lps[i] = 0;
                i++;
            }
        }
    }
    return lps;
}

vector<int> KMPSearch(const string &text, const string &pattern) {
    vector<int> positions;
    int n = text.length();
    int m = pattern.length();

    vector<int> lps = computeLPS(pattern);
    int i = 0;
    int j = 0;

    while (i < n) {
        if (text[i] == pattern[j]) {
            i++;
            j++;
        }

        if (j == m) {
            positions.push_back(i - j);
            j = lps[j - 1];
        } else if (i < n && text[i] != pattern[j]) {
            if (j != 0) {
                j = lps[j - 1];
            } else {
                i++;
            }
        }
    }

    return positions;
}

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
    vector<int> bpos(m + 1, 0);

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

vector<int> RabinKarpSearch(const string &text, const string &pattern) {
    const int prime = 101;
    const int d = 256;
    vector<int> positions;

    int n = text.length();
    int m = pattern.length();
    int patternHash = 0;
    int textHash = 0;
    int h = 1;

    for (int i = 0; i < m - 1; i++) {
        h = (h * d) % prime;
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
            outFile.write(reinterpret_cast<const char *>(&transition.first), sizeof(transition.first));
            outFile.write(reinterpret_cast<const char *>(&transition.second), sizeof(transition.second));
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

vector<int> AutomatonMatcherWithDFA(const string &text,
                                    const unordered_map<int, unordered_map<char, int>> &dfa,
                                    int patternLength) {
    vector<int> positions;
    int state = 0;
    int n = text.length();

    for (int i = 0; i < n; i++) {
        if (dfa.at(state).count(text[i])) {
            state = dfa.at(state).at(text[i]);
        } else {
            state = 0;
        }

        if (state == patternLength) {
            positions.push_back(i - patternLength + 1);
        }
    }

    return positions;
}

long long measureExecutionTime(const string &text, const string &pattern,
                               vector<int> (*algorithm)(const string &, const string &)) {
    long long totalTime = 0;

    for (int i = 0; i < 10; ++i) {
        auto start = high_resolution_clock::now();
        algorithm(text, pattern);

        auto stop = high_resolution_clock::now();
        totalTime += duration_cast<microseconds>(stop - start).count();
    }

    return totalTime / 10;
}

long long measureExecutionTimeWithDFA(
    const string &text,
    const unordered_map<int, unordered_map<char, int>> &dfa,
    int patternLength,
    vector<int> (*algorithm)(const string &, const unordered_map<int, unordered_map<char, int>> &, int)) {

    long long totalTime = 0;

    for (int i = 0; i < 50; ++i) {
        auto start = high_resolution_clock::now();
        algorithm(text, dfa, patternLength);
        auto stop = high_resolution_clock::now();
        totalTime += duration_cast<microseconds>(stop - start).count();
    }

    return totalTime / 50;
}

int main() {
    ifstream inputFile("input.txt");
    if (!inputFile.is_open()) {
        cerr << "Error: Could not open input file!" << endl;
        return 1;
    }

    ofstream outputFile("results.csv");
    if (!outputFile.is_open()) {
        cerr << "Error: Could not open output file!" << endl;
        return 1;
    }
    outputFile << "Algorithm,Testcase,PatternOccurrences,Time(us)\n";

    string line;
    vector<tuple<string, int, int, long long>> results;

    int testcaseNumber = 0;

    while (getline(inputFile, line)) {
        stringstream ss(line);
        string text, pattern;
        if (getline(ss, text, ',') && getline(ss, pattern, ',')) {
            int testcase = ++testcaseNumber;

            vector<pair<string, vector<int> (*)(const string &, const string &)>> algorithms = {
                {"BruteForce", BruteForceSearch},
                {"BoyerMoore", BoyerMooreSearch},
                {"KMP", KMPSearch},
                {"RabinKarp", RabinKarpSearch}};

            for (auto &algo : algorithms) {
                auto positions = algo.second(text, pattern);
                int occurrences = positions.size();
                long long execTime = measureExecutionTime(text, pattern, algo.second);

                results.push_back({algo.first, testcase, occurrences, execTime});
            }

            // string dfaFilename = "dfa\\dfa_" + to_string(testcase) + ".bin";
            // unordered_map<int, unordered_map<char, int>> dfa;

            // if (ifstream(dfaFilename)) {
            //     cout << "Loading DFA for testcase " << testcase << " from file " << dfaFilename << endl;
            //     dfa = LoadDFAFromFile(dfaFilename);
            // } else {
            //     cout << "Building DFA for testcase " << testcase << " and saving to file " << dfaFilename << endl;
            //     dfa = BuildDFA(pattern);
            //     SaveDFAToFile(dfa, dfaFilename);
            // }

            // auto positions = AutomatonMatcherWithDFA(text, dfa, pattern.length());
            // int occurrences = positions.size();
            // long long execTime = measureExecutionTimeWithDFA(
            //     text, dfa, pattern.length(), AutomatonMatcherWithDFA);

            // results.push_back({"AutomatonMatcher", testcase, occurrences, execTime});
        }
    }

    inputFile.close();

    sort(results.begin(), results.end(), [](const auto &a, const auto &b) {
        return get<1>(a) < get<1>(b);
    });

    for (const auto &[algoName, testcaseName, occurrences, execTime] : results) {
        outputFile << algoName << "," << testcaseName << "," << occurrences << "," << execTime << "\n";
    }

    outputFile.close();
    cout << "Results written to results.csv" << endl;

    return 0;
}