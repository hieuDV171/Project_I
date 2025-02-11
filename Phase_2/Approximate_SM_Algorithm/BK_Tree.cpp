#include <cmath>
#include <cstring>
#include <fstream>
#include <iostream>
#include <memory>
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

struct BKTreeNode {
    string word;
    unordered_map<int, unique_ptr<BKTreeNode>> children;

    BKTreeNode(const string &w) : word(w) {}
};

int LevenshteinDistance(const string &s1, const string &s2) {
    int len1 = s1.length(), len2 = s2.length();
    vector<vector<int>> dp(len1 + 1, vector<int>(len2 + 1));

    for (int i = 0; i <= len1; i++)
        dp[i][0] = i;
    for (int j = 0; j <= len2; j++)
        dp[0][j] = j;

    for (int i = 1; i <= len1; i++) {
        for (int j = 1; j <= len2; j++) {
            int cost = (s1[i - 1] == s2[j - 1]) ? 0 : 1;
            dp[i][j] = min(dp[i - 1][j] + 1, min(dp[i][j - 1] + 1, dp[i - 1][j - 1] + cost));
        }
    }
    return dp[len1][len2];
}

class BKTree {
private:
    unique_ptr<BKTreeNode> root;

public:
    BKTree() = default;

    void insert(const string &word) {
        if (!root) {
            root = make_unique<BKTreeNode>(word);
            return;
        }

        BKTreeNode *current = root.get();
        while (true) {
            int distance = LevenshteinDistance(word, current->word);
            if (current->children.find(distance) == current->children.end()) {
                current->children[distance] = make_unique<BKTreeNode>(word);
                break;
            }
            current = current->children[distance].get();
        }
    }

    vector<string> search(const string &word, int maxDistance) {
        vector<string> results;
        if (!root)
            return results;

        queue<BKTreeNode *> nodes;
        nodes.push(root.get());

        while (!nodes.empty()) {
            BKTreeNode *current = nodes.front();
            nodes.pop();

            int distance = LevenshteinDistance(word, current->word);
            if (distance <= maxDistance) {
                results.push_back(current->word);
            }

            for (const auto &[key, child] : current->children) {
                if (key >= distance - maxDistance && key <= distance + maxDistance) {
                    nodes.push(child.get());
                }
            }
        }

        return results;
    }

    void saveToFile(const string &filename) {
        ofstream outFile(filename, ios::binary);
        if (!outFile.is_open()) {
            cerr << "Error: Could not open file for saving!" << endl;
            return;
        }
        saveNode(root.get(), outFile);
        outFile.close();
    }

    void saveNode(BKTreeNode *node, ofstream &outFile) {
        size_t wordSize = node->word.size();
        outFile.write(reinterpret_cast<char *>(&wordSize), sizeof(wordSize));
        outFile.write(node->word.c_str(), wordSize);

        size_t numChildren = node->children.size();
        outFile.write(reinterpret_cast<char *>(&numChildren), sizeof(numChildren));

        for (const auto &[key, child] : node->children) {
            outFile.write(reinterpret_cast<const char *>(&key), sizeof(key));
            saveNode(child.get(), outFile);
        }
    }

    void loadFromFile(const string &filename) {
        ifstream inFile(filename, ios::binary);
        if (!inFile.is_open()) {
            cerr << "Error: Could not open file for loading!" << endl;
            return;
        }
        root = make_unique<BKTreeNode>("");
        loadNode(root.get(), inFile);
        inFile.close();
    }

    void loadNode(BKTreeNode *node, ifstream &inFile) {
        size_t wordSize;
        inFile.read(reinterpret_cast<char *>(&wordSize), sizeof(wordSize));
        node->word.resize(wordSize);
        inFile.read(&node->word[0], wordSize);

        size_t numChildren;
        inFile.read(reinterpret_cast<char *>(&numChildren), sizeof(numChildren));

        for (size_t i = 0; i < numChildren; ++i) {
            int key;
            inFile.read(reinterpret_cast<char *>(&key), sizeof(key));
            node->children[key] = make_unique<BKTreeNode>("");
            loadNode(node->children[key].get(), inFile);
        }
    }
};

int main() {
    BKTree tree;

    ifstream binFile("bk_tree.dat", ios::binary);
    if (binFile.is_open()) {
        tree.loadFromFile("bk_tree.dat");
        binFile.close();
        cout << "Loaded BK-Tree from binary file." << endl;
    } else {
        ifstream inputFile("words_alpha.txt");
        if (!inputFile.is_open()) {
            cerr << "Error: Could not open dictionary.txt!" << endl;
            return 1;
        }

        string word;
        while (getline(inputFile, word)) {
            tree.insert(word);
        }
        inputFile.close();

        tree.saveToFile("bk_tree.dat");
        cout << "Built and saved BK-Tree to binary file." << endl;
    }

    string query;
    cout << "Enter a word to search: ";
    cin >> query;

    int maxDistance;
    cout << "Insert number of different word allowed (1 recommended): ";
    cin >> maxDistance;

    vector<string> results = tree.search(query, maxDistance);

    if (!results.empty()) {
        cout << "Similar words found:" << endl;
        for (const string &result : results) {
            cout << "  - " << result;
            if (result.compare(query) == 0) {
                cout << " (*)";
            }
            cout << endl;
        }
    } else {
        cout << "No similar words found." << endl;
    }

    return 0;
}