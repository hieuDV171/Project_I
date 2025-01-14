// Levenshtein has not designed to search in large text
#include <climits>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Calculates the Levenshtein distance between two strings
int LevenshteinDistance(const string &s1, const string &s2) {
    int len1 = s1.length();
    int len2 = s2.length();
    vector<vector<int>> dp(len1 + 1, vector<int>(len2 + 1));

    for (int i = 0; i <= len1; ++i) {
        dp[i][0] = i;
    }
    for (int j = 0; j <= len2; ++j) {
        dp[0][j] = j;
    }

    for (int i = 1; i <= len1; ++i) {
        for (int j = 1; j <= len2; ++j) {
            int cost = (s1[i - 1] == s2[j - 1]) ? 0 : 1;
            dp[i][j] = min(dp[i - 1][j] + 1, dp[i][j - 1] + 1);
            dp[i][j] = min(dp[i][j], dp[i - 1][j - 1] + cost);
        }
    }

    return dp[len1][len2];
}

// Finds all approximate occurrences of a pattern in a text with a maximum Levenshtein distance
vector<int> ApproximateSearch(const string &text, const string &pattern, int maxDistance) {
    vector<int> positions;
    int textLength = text.length();
    int patternLength = pattern.length();

    for (int i = 0; i <= textLength - patternLength; ++i) {
        string substring = text.substr(i, patternLength);
        int distance = LevenshteinDistance(substring, pattern);

        if (distance <= maxDistance) {
            positions.push_back(i);
        }
    }

    return positions;
}

int main() {
    ifstream inputFile("Fuzzy_input.txt");
    if (!inputFile.is_open()) {
        cerr << "Error: Could not open input file!" << endl;
        return 1;
    }

    int testcaseNumber = 1;
    string line;
    while (getline(inputFile, line)) {
        cout << "Testcase " << testcaseNumber << ":" << endl;

        size_t commaPos = line.find(',');
        if (commaPos == string::npos) {
            cerr << "Invalid input line: " << line << endl;
            continue;
        }

        string text = line.substr(0, commaPos);
        string pattern = line.substr(commaPos + 1);

        int maxDistance = 1; // Maximum allowed edit distance

        // Find approximate matches in the text
        vector<int> positions = ApproximateSearch(text, pattern, maxDistance);

        // Print the results
        if (positions.empty()) {
            cout << "No matches found!" << endl;
        } else {
            cout << "Pattern found at positions: ";
            for (int pos : positions) {
                cout << pos << " ";
            }
            cout << endl;
        }
        testcaseNumber++;
    }

    inputFile.close();
    return 0;
}