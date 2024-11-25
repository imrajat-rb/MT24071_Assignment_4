#include "InstituteSystem.h"

// Author class implementation
Author::Author(const string& name, const string& affiliation)
    : name(name), affiliation(affiliation) {}

string Author::getName() const { return name; }

string Author::getAffiliation() const { return affiliation; }

// InstituteAuthor class implementation
InstituteAuthor::InstituteAuthor(const string& name, const string& affiliation)
    : Author(name, affiliation) {}

// Publication class implementation
Publication::Publication(const string& title, const string& venue, const vector<Author>& authors, int year, const string& doi)
    : title(title), venue(venue), authors(authors), year(year), doi(doi) {}

bool Publication::hasInstituteAuthor(const string& instituteName) const {
    for (const auto& author : authors) {
        if (author.getAffiliation() == instituteName) {
            return true;
        }
    }
    return false;
}

void Publication::display() const {
    cout << "Title: " << title << endl;
    cout << "Venue: " << venue << endl;
    cout << "Year: " << year << endl;
    cout << "Authors: ";
    for (const auto& author : authors) {
        cout << author.getName() << " (" << author.getAffiliation() << "); ";
    }
    cout << endl;
    if (!doi.empty()) {
        cout << "DOI: " << doi << endl;
    }
    cout << "----------------------------------------" << endl;
}

// Institute class implementation
Institute::Institute(const string& name) : name(name) {}

void Institute::addPublication(const Publication& publication) {
    if (publication.hasInstituteAuthor(name)) {
        publications.push_back(publication);
    } else {
        cerr << "Error: At least one author must have affiliation with " << name << endl;
    }
}

void Institute::displayPublications() const {
    cout << "Publications for Institute: " << name << endl;
    cout << "========================================" << endl;
    for (const auto& publication : publications) {
        publication.display();
    }
}
