#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

using namespace std;

string generateRandomString(int length, char start = 'a', char end = 'z') {
    ostringstream oss;
    for (int i = 0; i < length; i++) {
        oss << static_cast<char>(start + rand() % (end - start + 1));
    }
    return oss.str();
}

void insertPatternMultipleTimes(string &text, const string &pattern, int occurrences) {
    unordered_set<int> usedPositions;
    int textLength = text.length();
    int patternLength = pattern.length();

    while (occurrences > 0) {
        int pos = rand() % (textLength - patternLength);

        if (usedPositions.find(pos) == usedPositions.end()) {
            text.replace(pos, patternLength, pattern);
            usedPositions.insert(pos);
            occurrences--;
        }
    }
}

void generateMultiPatternTestCases(const string &filename, int numTests) {
    ofstream outputFile(filename);
    if (!outputFile.is_open()) {
        cerr << "Error: Could not open output file!" << endl;
        return;
    }

    srand(time(0));

    for (int i = 1; i <= numTests; i++) {
        string text;
        vector<string> patterns;

        int numPatterns = 0;
        if (i <= 3) {
            text = generateRandomString(1000);
            numPatterns = 5;
        } else if (i <= 6) {
            text = generateRandomString(5000);
            numPatterns = 10;
        } else if (i <= 10) {
            text = generateRandomString(10000);
            numPatterns = 15;
        } else if (i <= 15) {
            text = generateRandomString(20000);
            numPatterns = 20;
        } else {
            text = generateRandomString(50000);
            numPatterns = 50;
        }

        for (int j = 0; j < numPatterns; j++) {
            string pattern = generateRandomString(5 + rand() % 10);
            patterns.push_back(pattern);
            int occurrences = 2 + rand() % 4;
            insertPatternMultipleTimes(text, pattern, occurrences);
        }

        ostringstream patternStream;
        for (size_t j = 0; j < patterns.size(); j++) {
            patternStream << patterns[j];
            if (j != patterns.size() - 1) {
                patternStream << ",";
            }
        }

        outputFile << text << "," << patternStream.str() << "\n";
    }

    outputFile.close();
    cout << "Multi-pattern test cases written to " << filename << endl;
}

int main() {
    generateMultiPatternTestCases("MP_input.txt", 20);
    return 0;
}