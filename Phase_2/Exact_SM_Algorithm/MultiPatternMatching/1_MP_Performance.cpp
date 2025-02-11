#include <algorithm>
#include <chrono>
#include <fstream>
#include <functional>
#include <iostream>
#include <queue>
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

struct TrieNodeAC {
    unordered_map<char, TrieNodeAC *> children;
    TrieNodeAC *fail = nullptr;
    vector<string> output;
};

template <typename NodeType>
void InsertTrie(NodeType *root, const string &pattern) {};

template <>
void InsertTrie(TrieNode *root, const string &pattern) {
    TrieNode *node = root;
    for (char ch : pattern) {
        if (!node->children[ch]) {
            node->children[ch] = new TrieNode();
        }
        node = node->children[ch];
    }
    node->isEndOfWord = true;
}

template <>
void InsertTrie(TrieNodeAC *root, const string &pattern) {
    TrieNodeAC *node = root;
    for (char ch : pattern) {
        if (!node->children[ch]) {
            node->children[ch] = new TrieNodeAC();
        }
        node = node->children[ch];
    }
    node->output.push_back(pattern);
}

template <typename NodeType>
unordered_map<string, vector<int>> Search(NodeType *root, const string &text);

template <>
unordered_map<string, vector<int>> Search(TrieNode *root, const string &text) {
    unordered_map<string, vector<int>> result;
    int n = text.length();

    for (int i = 0; i < n; i++) {
        TrieNode *node = root;
        string currentPattern;
        for (int j = i; j < n; j++) {
            char ch = text[j];
            if (!node->children[ch]) {
                break;
            }
            node = node->children[ch];
            currentPattern += ch;
            if (node->isEndOfWord) {
                result[currentPattern].push_back(i);
            }
        }
    }
    return result;
}

template <>
unordered_map<string, vector<int>> Search(TrieNodeAC *root, const string &text) {
    unordered_map<string, vector<int>> result;
    TrieNodeAC *node = root;

    for (int i = 0; i < text.length(); i++) {
        char ch = text[i];

        while (node != root && !node->children.count(ch)) {
            node = node->fail;
        }

        if (node->children.count(ch)) {
            node = node->children[ch];
        }

        for (const string &pattern : node->output) {
            result[pattern].push_back(i - pattern.length() + 1);
        }
    }
    return result;
}

void BuildFailureLinks(TrieNodeAC *root) {
    queue<TrieNodeAC *> q;
    root->fail = root;

    for (auto &[ch, child] : root->children) {
        child->fail = root;
        q.push(child);
    }

    while (!q.empty()) {
        TrieNodeAC *current = q.front();
        q.pop();

        for (auto &[ch, child] : current->children) {
            TrieNodeAC *failNode = current->fail;

            while (failNode != root && !failNode->children.count(ch)) {
                failNode = failNode->fail;
            }

            if (failNode->children.count(ch)) {
                child->fail = failNode->children[ch];
            } else {
                child->fail = root;
            }

            child->output.insert(child->output.end(),
                                 child->fail->output.begin(),
                                 child->fail->output.end());
            q.push(child);
        }
    }
}

unordered_map<string, vector<int>> RabinKarpSearch(const string &text, const vector<string> &patterns) {
    const int prime = INT_MAX;
    const int d = 256;
    unordered_map<string, vector<int>> patternPositions;

    for (const string &pattern : patterns) {
        int n = text.length();
        int m = pattern.length();
        int patternHash = 0;
        int textHash = 0;
        int h = 1;

        // Compute the value of h = pow(d, m-1) % prime
        for (int i = 0; i < m - 1; i++) {
            h = (h * d) % prime;
        }

        // Calculate the initial hashes for the pattern and the first window of the text
        for (int i = 0; i < m; i++) {
            patternHash = (d * patternHash + pattern[i]) % prime;
            textHash = (d * textHash + text[i]) % prime;
        }

        // Search for the pattern in the text
        for (int i = 0; i <= n - m; i++) {
            if (patternHash == textHash) {
                bool match = true;
                // Verify the match by comparing the pattern with the substring
                for (int j = 0; j < m; j++) {
                    if (text[i + j] != pattern[j]) {
                        match = false;
                        break;
                    }
                }
                if (match) {
                    patternPositions[pattern].push_back(i);
                }
            }

            // Compute hash for the next window
            if (i < n - m) {
                textHash = (d * (textHash - text[i] * h) + text[i + m]) % prime;
                if (textHash < 0) {
                    textHash += prime;
                }
            }
        }
    }

    return patternPositions;
}

template <typename NodeType>
pair<long long, int> measureExecutionTime(NodeType *root, const string &text,
                                          unordered_map<string, vector<int>> (*algorithm)(NodeType *, const string &)) {
    long long totalTime = 0;
    int num = 9;

    auto start = high_resolution_clock::now();
    unordered_map<string, vector<int>> result = algorithm(root, text);
    auto stop = high_resolution_clock::now();
    totalTime += duration_cast<microseconds>(stop - start).count();

    int totalOccurrences = 0;
    for (const auto &[pattern, positions] : result) {
        totalOccurrences += positions.size();
    }

    for (int i = 0; i < num; ++i) {
        start = high_resolution_clock::now();
        algorithm(root, text);
        stop = high_resolution_clock::now();
        totalTime += duration_cast<microseconds>(stop - start).count();
    }

    return {totalTime / num + 1, totalOccurrences};
}

pair<long long, int> measureExecutionTimeRabinKarp(const string &text, const vector<string> &patterns) {
    long long totalTime = 0;
    int num = 9;

    auto start = high_resolution_clock::now();
    unordered_map<string, vector<int>> result = RabinKarpSearch(text, patterns);
    auto stop = high_resolution_clock::now();
    totalTime += duration_cast<microseconds>(stop - start).count();

    int totalOccurrences = 0;
    for (const auto &[pattern, positions] : result) {
        totalOccurrences += positions.size();
    }

    for (int i = 0; i < num; ++i) {
        start = high_resolution_clock::now();
        RabinKarpSearch(text, patterns);
        stop = high_resolution_clock::now();
        totalTime += duration_cast<microseconds>(stop - start).count();
    }

    return {totalTime / num + 1, totalOccurrences};
}

int main() {
    ifstream inputFile("MP_input.txt");
    if (!inputFile.is_open()) {
        cerr << "Error: Could not open input file!" << endl;
        return 1;
    }

    TrieNode *basicRoot = new TrieNode();
    TrieNodeAC *ahoRoot = new TrieNodeAC();

    vector<tuple<string, function<pair<long long, int>(const string &, const vector<string> &)>>> algorithms = {
        {"BasicTrie", [&](const string &text, const vector<string> &patterns) -> pair<long long, int> {
             for (const auto &pattern : patterns) {
                 InsertTrie(basicRoot, pattern);
             }
             return measureExecutionTime(basicRoot, text, Search<TrieNode>);
         }},
        {"AhoCorasick", [&](const string &text, const vector<string> &patterns) -> pair<long long, int> {
             for (const auto &pattern : patterns) {
                 InsertTrie(ahoRoot, pattern);
             }
             BuildFailureLinks(ahoRoot);
             return measureExecutionTime(ahoRoot, text, Search<TrieNodeAC>);
         }},
        // Thêm mục cho Rabin-Karp
        {"RabinKarp", [&](const string &text, const vector<string> &patterns) -> pair<long long, int> {
             return measureExecutionTimeRabinKarp(text, patterns);
         }}
        };

    ofstream outputFile("MP_results.csv");
    if (!outputFile.is_open()) {
        cerr << "Error: Could not open output file!" << endl;
        return 1;
    }
    outputFile << "Algorithm,Testcase,PatternCount,PatternOccurrences,Time(us)\n";

    string line;
    vector<tuple<string, int, int, int, long long>> results;

    int testcaseNumber = 0;

    while (getline(inputFile, line)) {
        stringstream ss(line);
        string text, pattern;
        vector<string> patterns;

        if (getline(ss, text, ',')) {
            // Đọc các mẫu
            while (getline(ss, pattern, ',')) {
                patterns.push_back(pattern);
            }

            int testcase = ++testcaseNumber;

            for (auto &[algoName, algoFunc] : algorithms) {
                auto [execTime, totalOccurrences] = algoFunc(text, patterns);
                results.push_back({algoName, testcase, static_cast<int>(patterns.size()), totalOccurrences, execTime});
            }
        }
    }

    inputFile.close();

    sort(results.begin(), results.end(), [](const auto &a, const auto &b) {
        return get<1>(a) < get<1>(b);
    });

    for (const auto &[algoName, testcase, patternCount, totalOccurrences, execTime] : results) {
        outputFile << algoName << "," << testcase << "," << patternCount << "," << totalOccurrences << "," << execTime << "\n";
    }

    outputFile.close();
    cout << "Results written to MP_results.csv" << endl;

    return 0;
}