#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

struct TrieNode {
    unordered_map<char, TrieNode *> children;
    TrieNode *fail = nullptr;
    vector<string> output;
};

void InsertTrie(TrieNode *root, const string &pattern) {
    TrieNode *node = root;
    for (char ch : pattern) {
        if (!node->children[ch]) {
            node->children[ch] = new TrieNode();
        }
        node = node->children[ch];
    }
    node->output.push_back(pattern);
}

void BuildFailureLinks(TrieNode *root) {
    queue<TrieNode *> q;
    root->fail = root;

    for (auto &[ch, child] : root->children) {
        child->fail = root;
        q.push(child);
    }

    while (!q.empty()) {
        TrieNode *current = q.front();
        q.pop();

        for (auto &[ch, child] : current->children) {
            TrieNode *failNode = current->fail;

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

unordered_map<string, vector<int>> AhoCorasickSearch(TrieNode *root, const string &text) {
    unordered_map<string, vector<int>> result;
    TrieNode *node = root;

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

        testCases.emplace_back(text, patterns);
    }
    inputFile.close();

    BuildFailureLinks(root);

    cout << "Searching patterns in the text using Aho-Corasick..." << endl;
    for (const auto &[text, patterns] : testCases) {
        unordered_map<string, vector<int>> patternPositions = AhoCorasickSearch(root, text);

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