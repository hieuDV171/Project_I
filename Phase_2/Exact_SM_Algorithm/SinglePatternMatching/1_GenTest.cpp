#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

// Function to generate a random string of specified length
string generateRandomString(int length, char start = 'a', char end = 'z') {
    ostringstream oss;
    for (int i = 0; i < length; i++) {
        oss << static_cast<char>(start + rand() % (end - start + 1));
    }
    return oss.str();
}

// Function to generate complex test cases
void generateComplexTestCases(const string &filename, int numTests) {
    ofstream outputFile(filename);
    if (!outputFile.is_open()) {
        cerr << "Error: Could not open output file!" << endl;
        return;
    }

    srand(time(0)); // Seed random number generator

    for (int i = 1; i <= numTests; i++) {
        ostringstream textStream;
        ostringstream patternStream;
        string text;
        string pattern;

        if (i <= 3) {
            // Simple cases: Small text and pattern
            text = generateRandomString(1000);
            pattern = generateRandomString(20);
            int pat_len = pattern.length();

            for (int j = 0; j < 3; j++) {
                patternStream << pattern.substr(0, rand() % pat_len);
            }
            pattern += patternStream.str();

            for (int j = 0; j < 10; j++) {
                textStream << pattern;
            }
        } else if (i <= 6) {
            // Partial repetition: Pattern is repeated with variations
            pattern = generateRandomString(80);
            int pat_len = pattern.length();
            text = generateRandomString(5000);

            for (int j = 0; j < 5; j++) {
                patternStream << pattern.substr(0, rand() % pat_len);
            }
            pattern += patternStream.str();

            for (int j = 0; j < 100; j++) {
                if (rand() % 2) {
                    textStream << pattern << generateRandomString(rand() % 10);
                } else {
                    textStream << pattern.substr(0, rand() % pat_len);
                }
            }
        } else if (i <= 10) {
            // Full repetition: Text contains multiple full occurrences of the pattern
            pattern = generateRandomString(100);
            int pat_len = pattern.length();

            for (int j = 0; j < 10; j++) {
                patternStream << pattern.substr(0, rand() % pat_len);
            }
            pattern += patternStream.str();

            text = pattern;
            for (int j = 0; j < 200; j++) {
                textStream << pattern;
            }
        } else if (i <= 15) {
            // Large text with mixed repetitions and noise
            text = generateRandomString(20000);
            pattern = generateRandomString(100);
            int pat_len = pattern.length();

            for (int j = 0; j < 20; j++) {
                patternStream << pattern.substr(0, rand() % pat_len);
            }
            pattern += patternStream.str();

            for (int j = 0; j < 300; j++) {
                if (rand() % 2) {
                    textStream << pattern << generateRandomString(10);
                } else {
                    textStream << pattern.substr(0, rand() % pat_len);
                }
            }
        } else {
            // Very large text and complex patterns
            text = generateRandomString(100000);
            pattern = generateRandomString(100);

            int pat_len = pattern.length();
            for (int j = 0; j < 30; j++) {
                patternStream << pattern.substr(0, rand() % pat_len);
            }
            pattern += patternStream.str();

            for (int j = 0; j < 1000; j++) {
                if (rand() % 3 == 0) {
                    textStream << pattern << generateRandomString(100);
                } else if (rand() % 3 == 1) {
                    textStream << pattern.substr(rand() % 500, 500); // Partial overlap
                } else {
                    textStream << generateRandomString(500); // Noise
                }
            }
        }

        text += textStream.str();
        // Write the generated test case to the file
        outputFile << text << "," << pattern << "\n";
    }

    outputFile.close();
    cout << "Complex test cases written to " << filename << endl;
}

int main() {
    generateComplexTestCases("input.txt", 20); // Generate 20 complex test cases
    return 0;
}