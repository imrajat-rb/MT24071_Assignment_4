#ifndef INSTITUTESYSTEM_H
#define INSTITUTESYSTEM_H

#include <string>
#include <vector>
#include <iostream>
using namespace std;

// Author class
class Author {
private:
    string name;
    string affiliation;

public:
    Author(const string& name, const string& affiliation);
    string getName() const;
    string getAffiliation() const;
};

// InstituteAuthor class (derived)
class InstituteAuthor : public Author {
public:
    InstituteAuthor(const string& name, const string& affiliation);
};

// Publication class
class Publication {
private:
    string title;
    string venue;
    vector<Author> authors;
    string doi;
    int year;

public:
    Publication(const string& title, const string& venue, const vector<Author>& authors, int year, const string& doi = "");
    bool hasInstituteAuthor(const string& instituteName) const;
    void display() const;
};

// Institute class
class Institute {
private:
    string name;
    vector<Publication> publications;

public:
    Institute(const string& name);
    void addPublication(const Publication& publication);
    void displayPublications() const;
};

#endif // INSTITUTESYSTEM_H
