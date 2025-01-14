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

// Function to generate test cases with varying complexity
void generateTestCases(const string &filename, int numTests) {
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

        // Vary pattern length and text length for each test case
        int patternLength = rand() % 50 + 10;   // Pattern length between 10 and 60
        int textLength = rand() % 1000 + 100; // Text length between 100 and 1100

        text = generateRandomString(textLength);
        pattern = generateRandomString(patternLength);

        // Add some noise to the beginning and end of the text
        textStream << generateRandomString(rand() % 100);
        // Generate text with varying occurrences of the pattern and noise
        for (int j = 0; j < rand() % 10; j++) { // Number of pattern occurrences
            textStream << pattern;
            if (rand() % 2) { // Introduce some noise between occurrences
                textStream << generateRandomString(rand() % 50);
            }
        }

        textStream << generateRandomString(rand() % 100);

        text += textStream.str();

        // Write the generated test case to the file
        outputFile << text << "," << pattern << "\n";
    }

    outputFile.close();
    cout << "Test cases written to " << filename << endl;
}

int main() {
    generateTestCases("Fuzzy_input.txt", 30); // Generate 30 test cases
    return 0;
}