#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>
#include <math.h>

using namespace std;
using namespace chrono;
int maxDis = 1;

int EditDistance(const string &s1, const string &s2, bool allowTransposition) {
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
            dp[i][j] = min({dp[i - 1][j] + 1,          // Deletion
                            dp[i][j - 1] + 1,          // Insertion
                            dp[i - 1][j - 1] + cost}); // Substitution

            if (allowTransposition && i > 1 && j > 1 &&
                s1[i - 1] == s2[j - 2] && s1[i - 2] == s2[j - 1]) {
                dp[i][j] = min(dp[i][j], dp[i - 2][j - 2] + 1); // Transposition
            }
        }
    }

    return dp[len1][len2];
}

vector<int> ApproximateSearch(const string &text, const string &pattern, int maxDistance, bool allowTransposition) {
    vector<int> positions;
    int textLength = text.length();
    int patternLength = pattern.length();

    for (int i = 0; i <= textLength - patternLength; ++i) {
        string substring = text.substr(i, patternLength);
        int distance = EditDistance(substring, pattern, allowTransposition);

        if (distance <= maxDistance) {
            positions.push_back(i);
        }
    }

    return positions;
}

vector<int> LevenshteinSearch(const string &text, const string &pattern, int maxDistance) {
    return ApproximateSearch(text, pattern, maxDistance, false);
}

vector<int> DamerauLevenshteinSearch(const string &text, const string &pattern, int maxDistance) {
    return ApproximateSearch(text, pattern, maxDistance, true);
}

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

double mapDistanceToSimilarity(int maxDistance, double alpha = 1.0) {
    return max(0.0, 1.0 - alpha * static_cast<double>(maxDistance) / (3.0 + maxDistance));
}

vector<int> JaroWinklerSearch(const string &text, const string &pattern, int maxDistance) {
    vector<int> positions;
    int textLength = text.length();
    int patternLength = pattern.length();

    double minSimilarity = mapDistanceToSimilarity(maxDistance);

    for (int i = 0; i <= textLength - patternLength; ++i) {
        string substring = text.substr(i, patternLength);
        double similarity = JaroWinklerDistance(substring, pattern);
        if (similarity >= minSimilarity) {
            positions.push_back(i);
        }
    }

    return positions;
}

long long measureExecutionTime(const string &text, const string &pattern,
                               vector<int> (*algorithm)(const string &, const string &, int)) {
    long long totalTime = 0;
    const int numRuns = 5;

    for (int i = 0; i < numRuns; ++i) {
        auto start = high_resolution_clock::now();
        algorithm(text, pattern, maxDis);
        auto stop = high_resolution_clock::now();
        totalTime += duration_cast<microseconds>(stop - start).count();
    }

    return totalTime / numRuns;
}

int main() {
    ifstream inputFile("Fuzzy_input.txt");
    if (!inputFile.is_open()) {
        cerr << "Error: Could not open input file!" << endl;
        return 1;
    }

    vector<pair<string, vector<int> (*)(const string &, const string &, int)>> algorithms = {
        {"Levenshtein", LevenshteinSearch},
        {"DamerauLevenshtein", DamerauLevenshteinSearch},
        {"JaroWinkler", JaroWinklerSearch}
    };

    ofstream outputFile("Fuzzy_results.csv");
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

            for (auto &algo : algorithms) {
                auto positions = algo.second(text, pattern, maxDis);
                int occurrences = positions.size();
                long long execTime = measureExecutionTime(text, pattern, algo.second);

                results.push_back({algo.first, testcase, occurrences, execTime});
            }
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
    cout << "Results written to Fuzzy_results.csv" << endl;

    return 0;
}