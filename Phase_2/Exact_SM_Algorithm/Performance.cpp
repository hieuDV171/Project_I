#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

using namespace std;
using namespace chrono;

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

long long measureExecutionTime(const string &text, const string &pattern, vector<int> (*algorithm)(const string &, const string &)) {
    auto start = high_resolution_clock::now();
    algorithm(text, pattern);
    auto stop = high_resolution_clock::now();
    return duration_cast<nanoseconds>(stop - start).count();
}

int main() {
    ifstream inputFile("input.txt");
    if (!inputFile.is_open()) {
        cerr << "Error: Could not open input file!" << endl;
        return 1;
    }

    vector<pair<string, vector<int> (*)(const string &, const string &)>> algorithms = {
        {"BruteForce", BruteForceSearch}};

    ofstream outputFile("results.csv");
    if (!outputFile.is_open()) {
        cerr << "Error: Could not open output file!" << endl;
        return 1;
    }
    outputFile << "Algorithm,Testcase,PatternOccurrences,Time(ns)\n";

    string line;
    vector<tuple<string, int, int, long long>> results;

    int testcaseNumber = 0;

    while (getline(inputFile, line)) {
        stringstream ss(line);
        string text, pattern;
        if (getline(ss, text, ',') && getline(ss, pattern, ',')) {
            int testcase = ++testcaseNumber;

            for (auto &algo : algorithms) {
                auto positions = algo.second(text, pattern);
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
    cout << "Results written to results.csv" << endl;

    return 0;
}