#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

double JaroDistance(const string &s1, const string &s2) {
    int len1 = s1.length(), len2 = s2.length();
    if (len1 == 0)
        return len2 == 0 ? 1.0 : 0.0;

    int matchDistance = max(len1, len2) / 2 - 1;
    vector<bool> s1Matches(len1, false), s2Matches(len2, false);

    // Count matches
    int matches = 0, transpositions = 0;
    for (int i = 0; i < len1; i++) {
        for (int j = max(0, i - matchDistance); j < min(len2, i + matchDistance + 1); j++) {
            if (s1[i] == s2[j] && !s2Matches[j]) {
                s1Matches[i] = true;
                s2Matches[j] = true;
                matches++;
                break;
            }
        }
    }

    if (matches == 0)
        return 0.0;

    // Count transpositions
    int k = 0;
    for (int i = 0; i < len1; i++) {
        if (s1Matches[i]) {
            while (!s2Matches[k])
                k++;
            if (s1[i] != s2[k])
                transpositions++;
            k++;
        }
    }
    transpositions /= 2;

    return (matches / (double)len1 + matches / (double)len2 + (matches - transpositions) / (double)matches) / 3.0;
}

double JaroWinklerDistance(const string &s1, const string &s2, double scalingFactor = 0.1) {
    double jaro = JaroDistance(s1, s2);
    int prefixLength = 0;

    for (int i = 0; i < min(4, (int)min(s1.length(), s2.length())); i++) {
        if (s1[i] == s2[i])
            prefixLength++;
        else
            break;
    }

    return jaro + (scalingFactor * prefixLength * (1.0 - jaro));
}

vector<pair<int, double>> JaroWinklerSearch(const string &text, const string &pattern, double minSimilarity) {
    vector<pair<int, double>> results;
    int textLength = text.length();
    int patternLength = pattern.length();

    for (int i = 0; i <= textLength - patternLength; ++i) {
        string substring = text.substr(i, patternLength);
        double similarity = JaroWinklerDistance(substring, pattern);
        if (similarity >= minSimilarity) {
            results.emplace_back(i, similarity);
        }
    }

    return results;
}

int main() {
    ifstream inputFile("Fuzzy_input.txt");
    if (!inputFile.is_open()) {
        cerr << "Error: Could not open input file!" << std::endl;
        return 1;
    }

    double minSimilarity = 0.9;
    string line;
    while (getline(inputFile, line)) {
        stringstream ss(line);
        string text, pattern;

        if (getline(ss, text, ',') && getline(ss, pattern, ',')) {
            vector<pair<int, double>> matches = JaroWinklerSearch(text, pattern, minSimilarity);
            if (!matches.empty()) {
                cout << "Text: \"" << text << "\", Pattern: \"" << pattern << "\"" << endl;
                cout << "Matching positions and similarities: " << endl;
                for (const auto &[pos, similarity] : matches) {
                    cout << "  Position: " << pos
                         << ", Similarity: " << fixed << setprecision(2) << (similarity * 100) << "%"
                         << endl;
                }
            } else {
                cout << "Text: \"" << text << "\", Pattern: \"" << pattern << "\" -> No matches found!" << endl;
            }
        }
    }

    inputFile.close();
    return 0;
}