#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

// Rabin-Karp Algorithm to search for multiple patterns
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

int main() {
    string line, text;
    vector<pair<string, vector<string>>> testCases;

    // Open the input file
    ifstream inputFile("MP_input.txt");
    if (!inputFile.is_open()) {
        cerr << "Error: Could not open input file!" << endl;
        return 1;
    }

    // Read the text and patterns from the file
    while (getline(inputFile, line)) {
        stringstream ss(line);
        string textPart;
        vector<string> patterns;

        // Read the text part (before comma)
        if (getline(ss, textPart, ',')) {
            text = textPart;

            // Read the patterns part (after comma)
            string pattern;
            while (getline(ss, pattern, ',')) {
                patterns.push_back(pattern);
            }

            // Store the text and its patterns
            testCases.push_back({text, patterns});
        }
    }

    // Close the input file
    inputFile.close();

    // Process each test case
    for (const auto &[text, patterns] : testCases) {
        unordered_map<string, vector<int>> patternPositions = RabinKarpSearch(text, patterns);

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