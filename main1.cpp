
#include "InstituteSystem.h"

int main() {
    string instituteName = "IIITD";
    Institute institute(instituteName);

    InstituteAuthor author1("Arani Bhattacharya", instituteName);
    Author author2("Abhishek Maji", "External University");
    Author author3("Jaya Prakash Verma Champati", "External University");
    Author author4("James Gross", "External University");

    Publication pub1(
        "Fast Efficient Online Selection of Sensors for Transmitter Localization",
        "International Conference on Communication Systems & Networks",
        {author1, author2, author3, author4},
        2022,
        "10.1109/COMSNETS53615.2022.9668385");

    Publication pub2(
        "A Gaussian Process Based Technique of Efficient Sensor Selection for Transmitter Localization",
        "IEEE International Symposium on Dynamic Spectrum Access Networks",
        {author1, author2},
        2021,
        "10.1109/DySPAN53946.2021.9677290");

    Publication pub3(
        "Flow-based Rate Maximization for Link Aggregation Enabled Hybrid LiFi-WiFi Network",
        "IEEE Transactions on Vehicular Technology",
        {Author("Saswati Paramita", "External University"), author1, Author("Rizwana Ahmad", "External University")},
        2024,
        "10.1109/TVT.2024.3477310");

    institute.addPublication(pub1);
    institute.addPublication(pub2);
    institute.addPublication(pub3);

    institute.displayPublications();

    return 0;
}
