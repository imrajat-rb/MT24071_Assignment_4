#include "InstituteSystem.h"
#include <iostream>
#include <fstream>
#include <regex>
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <stack>
#include <set>
#include <algorithm>

using namespace std;

// Function to check if braces in the entry are balanced
bool validateBraces(const string& entry) {
    stack<char> braces;
    for (char ch : entry) {
        if (ch == '{') {
            braces.push(ch);
        } else if (ch == '}') {
            if (braces.empty()) {
                return false; // Unmatched closing brace
            }
            braces.pop();
        }
    }
    return braces.empty(); // All opening braces should be matched
}

// Function to trim leading and trailing spaces from a string
string trim(const string& str) {
    size_t first = str.find_first_not_of(" ");
    if (first == string::npos) return ""; // No non-space character found
    size_t last = str.find_last_not_of(" ");
    return str.substr(first, last - first + 1);
}

// Function to split authors by "and" or "," and clean up whitespace
vector<string> splitAuthors(const string& authorField) {
    vector<string> authors;
    stringstream ss(authorField);
    string token;
    string delimiter = " and ";

    // Split by "and"
    while (getline(ss, token, ',')) {
        // Trim leading and trailing spaces
        token = trim(token);
        
        // Split by "and" in case of multiple authors in one part
        size_t pos;
        while ((pos = token.find(delimiter)) != string::npos) {
            authors.push_back(trim(token.substr(0, pos)));
            token = token.substr(pos + delimiter.length());
        }

        authors.push_back(trim(token)); // Add the last part
    }

    return authors;
}

// Function to load faculty list from a CSV file (only loading names from the first column)
set<string> loadFacultyList(const string& filename) {
    ifstream facultyFile(filename);
    if (!facultyFile.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        exit(1);
    }

    set<string> facultySet;
    string line;

    // Skip the header line
    getline(facultyFile, line);

    // Extract names from the CSV file
    while (getline(facultyFile, line)) {
        stringstream ss(line);
        string name;
        // Read the first column (Name)
        if (getline(ss, name, ',')) {
            name = trim(name); // Trim any extra spaces
            facultySet.insert(name);
        }
    }

    return facultySet;
}

// Function to parse key-value pairs from a BibTeX entry
map<string, string> parseBibTeXEntry(const string& entry) {
    map<string, string> bibData;

    // Regex to match key-value pairs
    regex kvRegex(R"((\w+)\s*=\s*\{(.*?)\},?)");
    smatch match;

    string::const_iterator searchStart(entry.cbegin());
    while (regex_search(searchStart, entry.cend(), match, kvRegex)) {
        string key = match[1];
        string value = match[2];
        bibData[key] = value;
        searchStart = match.suffix().first; // Move search start to after the current match
    }

    return bibData;
}

// Function to read and parse BibTeX entries from a file
vector<map<string, string>> parseBibFile(const string& filename) {
    ifstream bibFile(filename);
    if (!bibFile.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        exit(1);
    }

    vector<map<string, string>> parsedEntries;
    string line, currentEntry;
    int entryIndex = 0;

    while (getline(bibFile, line)) {
        // Check for entry boundaries
        if (line.find("@") == 0) {
            if (!currentEntry.empty()) {
                // Validate and parse the previous entry
                entryIndex++;
                if (validateBraces(currentEntry)) {
                    parsedEntries.push_back(parseBibTeXEntry(currentEntry));
                } else {
                    cout << "Invalid entry at Entry " << entryIndex << " due to braces mismatch" << endl;
                }
                currentEntry.clear();
            }
        }
        currentEntry += line + "\n"; // Accumulate lines for the current entry
    }

    // Validate and parse the last entry if any
    if (!currentEntry.empty()) {
        entryIndex++;
        if (validateBraces(currentEntry)) {
            parsedEntries.push_back(parseBibTeXEntry(currentEntry));
        } else {
            cout << "Invalid entry: Braces mismatch" << entryIndex << endl;
        }
    }

    return parsedEntries;
}

int main() {
    string bibFilename = "publist.bib"; // BibTeX file
    string facultyFilename = "faculty.csv"; // Faculty list CSV
    string outputFilename = "publications.txt"; // Output file for results

    // Load faculty list (names from the first column)
    set<string> facultySet = loadFacultyList(facultyFilename);

    // Parse the BibTeX file
    vector<map<string, string>> parsedEntries = parseBibFile(bibFilename);

    // Open output file to save results
    ofstream outputFile(outputFilename);
    if (!outputFile.is_open()) {
        cerr << "Error: Unable to open output file." << endl;
        return 1;
    }

    // Process and output the parsed entries
    outputFile << "Parsed BibTeX Entries:\n";
    cout << "Parsed BibTeX Entries:\n";

    for (size_t i = 0; i < parsedEntries.size(); ++i) {
        outputFile << "\nPublication " << (i + 1) << ":\n";
        cout << "\nPublication " << (i + 1) << ":\n";

        const auto& entry = parsedEntries[i];
        for (const auto& kv : entry) {
            outputFile << "- " << kv.first << ": " << kv.second << '\n';
            cout << "- " << kv.first << ": " << kv.second << '\n';
        }

        auto authorIt = entry.find("author");
        if (authorIt != entry.end()) {
            vector<string> authors = splitAuthors(authorIt->second);

            bool affiliated = false;
            set<string> seenAuthors; // To track duplicate authors
            bool hasDuplicate = false;

            for (const string& author : authors) {
                if (seenAuthors.count(author)) {
                    hasDuplicate = true;
                    break;
                }
                seenAuthors.insert(author);

                if (facultySet.count(author)) {
                    affiliated = true;
                }
            }

            if (hasDuplicate) {
                outputFile << "This entry is invalid: Author repeated.\n";
                cout << "This entry is invalid: Author repeated.\n";
            } else if (!affiliated) {
                outputFile << "This entry is invalid: No author is affiliated.\n";
                cout << "This entry is invalid: No author is affiliated.\n";
            } else {
                outputFile << "Correct Publication\n";
                cout << "Correct Publication\n";
            }
        }
        outputFile << '\n';
        cout << '\n';
    }

    outputFile.close();
    cout << "Results saved to " << outputFilename << endl;
    return 0;
}
