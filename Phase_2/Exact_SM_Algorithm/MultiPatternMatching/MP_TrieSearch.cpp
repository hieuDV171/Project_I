#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

struct TrieNode {
    unordered_map<char, TrieNode *> children;
    bool isEndOfWord = false;
};

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

unordered_map<string, vector<int>> TrieSearch(TrieNode *root, const string &text) {
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

int main() {
    ifstream inputFile("MP_input.txt");
    if (!inputFile.is_open()) {
        cerr << "Error: Could not open input file!" << endl;
        return 1;
    }

    TrieNode *root = new TrieNode();
    vector<pair<string, vector<string>>> testCases;

    string line;
    while (getline(inputFile, line)) {
        stringstream ss(line);
        string text, pattern;
        vector<string> patterns;

        // Đọc văn bản
        if (getline(ss, text, ',')) {
            while (getline(ss, pattern, ',')) {
                patterns.push_back(pattern);
                InsertTrie(root, pattern);
            }
        }

        // Lưu test case
        testCases.emplace_back(text, patterns);
    }
    inputFile.close();

    cout << "Searching patterns in the text..." << endl;
    for (const auto &[text, patterns] : testCases) {
        unordered_map<string, vector<int>> patternPositions = TrieSearch(root, text);

        cout << "Text: " << text << endl;
        for (const string &pattern : patterns) {
            cout << "Pattern \"" << pattern << "\" found at indices: ";
            if (patternPositions.count(pattern) && !patternPositions[pattern].empty()) {
                for (int pos : patternPositions[pattern]) {
                    cout << pos << " ";
                }
            } else {
                cout << "Not found";
            }
            cout << endl;
        }
        cout << endl;
    }

    return 0;
}