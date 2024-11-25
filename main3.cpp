

#include <iostream>
#include <fstream>
#include <regex>
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <set>
#include <iterator>

using namespace std;

// Function to trim leading and trailing spaces from a string
string trim(const string& str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    if (first == string::npos) return ""; // No non-space character found
    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, last - first + 1);
}

// Function to split authors by "and" or "," and clean up whitespace
vector<string> splitAuthors(const string& authorField) {
    vector<string> authors;
    stringstream ss(authorField);
    string token;
    string delimiter = " and ";

    // Split by ","
    while (getline(ss, token, ',')) {
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
                parsedEntries.push_back(parseBibTeXEntry(currentEntry));
                currentEntry.clear();
            }
        }
        currentEntry += line + "\n"; // Accumulate lines for the current entry
    }

    // Validate and parse the last entry if any
    if (!currentEntry.empty()) {
        parsedEntries.push_back(parseBibTeXEntry(currentEntry));
    }

    return parsedEntries;
}

int main(int argc, char* argv[]) {
    // Check if at least one author name is provided
    if (argc < 2) {
        cerr << "Please provide at least one author name as an argument!" << endl;
        return 1;
    }

    // Collect the author names from command-line arguments
    vector<string> authorsToSearch;
    for (int i = 1; i < argc; ++i) {
        authorsToSearch.push_back(trim(argv[i]));
    }

    string bibFilename = "publist.bib"; // Replace with your BibTeX file path
    string facultyFilename = "faculty.csv"; // Replace with your faculty list file path

    // Load faculty list (names from the first column)
    set<string> facultySet = loadFacultyList(facultyFilename);

    // Parse the BibTeX file
    vector<map<string, string>> parsedEntries = parseBibFile(bibFilename);

    // For each author, display their publications and calculate average co-authors
    for (const string& author : authorsToSearch) {
        cout << "\n" << author << "'s Publications:\n";

        int totalCoAuthors = 0;
        int publicationCount = 0;

        // Iterate through all parsed BibTeX entries
        for (size_t i = 0; i < parsedEntries.size(); ++i) {
            const auto& entry = parsedEntries[i];
            auto authorIt = entry.find("author");

            if (authorIt != entry.end()) {
                vector<string> authors = splitAuthors(authorIt->second);

                // If the given author is in this paper, display the publication
                if (find(authors.begin(), authors.end(), author) != authors.end()) {
                    cout << publicationCount + 1 << ". \"" << entry.at("title") << "\" (Year: " << entry.at("year") << ")\n";
                    cout << "   Co-authors: ";

                    // Print co-authors and calculate total co-authors
                    vector<string> coAuthors;
                    for (const string& a : authors) {
                        if (a != author) {
                            coAuthors.push_back(a);
                        }
                    }

                    totalCoAuthors += coAuthors.size();
                    publicationCount++;

                    // Print co-authors
                    for (size_t i = 0; i < coAuthors.size(); ++i) {
                        cout << coAuthors[i];
                        if (i < coAuthors.size() - 1) {
                            cout << ", ";
                        }
                    }
                    cout << "\n";
                }
            }
        }

        // Calculate and print the average number of co-authors for the given author
        if (publicationCount > 0) {
            double averageCoAuthors = static_cast<double>(totalCoAuthors) / publicationCount;
            cout << author << "'s Average Number of Co-authors: " << averageCoAuthors << "\n";
        } else {
            cout << "No publications found for " << author << ".\n";
        }
    }

    return 0;
}
