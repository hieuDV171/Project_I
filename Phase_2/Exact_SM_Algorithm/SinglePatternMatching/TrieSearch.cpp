// Trie are optimized for searching multiple patterns at once.
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <iomanip>

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

vector<int> TrieSearch(const string &text, const string &pattern) {
    TrieNode *root = new TrieNode();

    InsertTrie(root, pattern);

    vector<int> positions;
    int n = text.length();

    for (int i = 0; i < n; i++) {
        TrieNode *node = root;
        for (int j = i; j < n; j++) {
            char ch = text[j];
            if (!node->children[ch]) {
                break;
            }
            node = node->children[ch];
            if (node->isEndOfWord) {
                positions.push_back(i);
                break;
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
            vector<int> positions = TrieSearch(text, pattern);
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