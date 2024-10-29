#include "AddressBook.h"
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <sstream>

AddressBook::AddressBook(string FileName) : filename(FileName) {}

void AddressBook::addAddress(Address* address) {
    addresses.push_back(address);
    // Â³äêðèâàºìî ôàéë äëÿ äîäàâàííÿ
    ofstream file(filename, ios::app); // Â³äêðèâàºìî ôàéë â ðåæèì³ "äîäàòè"
    if (file.is_open()) {
        // Çàïèñóºìî àäðåñó ó ôàéë
        cout << endl;
        file << address->toString() << endl; // Çàïèñ ó ôàéë
        file.close(); // Çàêðèâàºìî ôàéë
    }
    else {
        cerr << "Error: " << filename << endl;
    }
}

void AddressBook::loadAddressesFromFile() {
    ifstream file(filename); // Â³äêðèâàºìî ôàéë
    if (!file.is_open()) { // Ïåðåâ³ðêà íà â³äêðèòòÿ
        cerr << "Error: " << filename << endl;
        return; // Âèõ³ä ç ôóíêö³¿, ÿêùî ôàéë íå â³äêðèòî
    }
    string line;
    // ×èòàºìî ôàéë ðÿäîê çà ðÿäêîì
    while (getline(file, line)) {
        cout << line << endl; // Âèâîäèìî ðÿäîê íà åêðàí
    }

    file.close(); // Çàêðèâàºìî ôàéë
}

void AddressBook::analyzeAddresses() {
    int privateHouseCount = 0;
    int appartmentCount = 0;
    int buildingCount = 0;
    map<int, int> appartmentDistribution;

    // First verify we have selected indices
    if (selectedIndices.empty()) {
        cout << "No addresses selected for analysis. Please select addresses first." << endl;
        return;
    }

    // Read and process the file to match with selected indices
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return;
    }

    string line;
    vector<string> fileLines;

    // Store all lines from file
    while (getline(file, line)) {
        fileLines.push_back(line);
    }
    file.close();

    // Process selected addresses
    for (int index : selectedIndices) {
        // Check if index is valid for our file lines
        if (index >= 0 && index < fileLines.size()) {
            string addressLine = fileLines[index];

            // Parse the address line
            istringstream iss(addressLine);
            string addressType;
            iss >> addressType; // Get first word which should be the type

            if (addressType == "Private" || addressType == "PrivateHouse") {
                privateHouseCount++;
            }
            else if (addressType == "Apartment" || addressType == "Appartment") {
                appartmentCount++;

                // Extract building number from the address line
                int buildingNum = 0;
                string token;
                while (iss >> token) {
                    // Try to find building number in the address string
                    if (isdigit(token[0])) {
                        buildingNum = stoi(token);
                        break;
                    }
                }

                if (buildingNum > 0) {
                    appartmentDistribution[buildingNum]++;
                }
            }
        }
        else {
            cerr << "Warning: Invalid index " << index << " (out of range)" << endl;
        }
    }

    // Calculate building count from apartment distribution
    buildingCount = appartmentDistribution.size();

    // Output analysis results
    cout << "\nAnalysis Results:" << endl;
    cout << "------------------------" << endl;
    cout << "Number of private houses: " << privateHouseCount << endl;
    cout << "Number of apartments: " << appartmentCount << endl;
    cout << "Number of unique buildings: " << buildingCount << endl;

    if (buildingCount > 0) {
        double averageAppartments = static_cast<double>(appartmentCount) / buildingCount;
        cout << "Average number of apartments per building: " << fixed << setprecision(2)
            << averageAppartments << endl;

        // Display distribution of apartments per building
        cout << "\nApartments distribution by building:" << endl;
        for (const auto& pair : appartmentDistribution) {
            cout << "Building " << pair.first << ": " << pair.second << " apartment(s)" << endl;
        }
    }
    else if (appartmentCount > 0) {
        cout << "Warning: Apartments found but building numbers could not be determined." << endl;
    }

    // Verify all addresses were processed
    int totalProcessed = privateHouseCount + appartmentCount;
    if (totalProcessed < selectedIndices.size()) {
        cout << "\nWarning: " << (selectedIndices.size() - totalProcessed)
            << " selected address(es) could not be properly analyzed." << endl;
    }
    cout << endl;
}

const vector<Address*>& AddressBook::getAddresses() const {
    return addresses;
}

AddressBook::~AddressBook() {
    for (Address* address : addresses) {
        delete address;
    }
}

void AddressBook::displayAddresses() {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: " << filename << endl;
        return;
    }

    string line;
    int index = 1;
    // ×èòàºìî ôàéë ðÿäîê çà ðÿäêîì òà âèâîäèìî íà åêðàí
    while (getline(file, line)) {
        cout << index++ << ": " << line << endl; // Íóìåðóºìî àäðåñè
    }

    file.close();
}

void AddressBook::selectAddressesForMailing() {
    displayAddresses(); // Âèâîäèìî àäðåñè äëÿ âèáîðó

    cout << "Enter the address numbers (separated by a space) that you want to select for the mailing list: ";
    string input;
    getline(cin, input);

    istringstream iss(input);
    int number;

    selectedIndices.clear(); // Î÷èùóºìî ïîïåðåäí³ âèáîðè
    bool validInput = false; // Ôëàã äëÿ ïåðåâ³ðêè, ÷è áóëè ââåäåí³ âàë³äí³ íîìåðè

    // Ç÷èòóºìî íîìåðè, ââåäåí³ êîðèñòóâà÷åì
    while (iss >> number) {
        // Äîäàºìî äî selectedIndices, ÿêùî íîìåð ä³éñíèé
        if (number > 0) {
            selectedIndices.push_back(number - 1); // Çáåð³ãàºìî ³íäåêñ ó ôîðìàò³ 0-³íäåêñàö³¿
            validInput = true;
        }
    }

    // Ïåðåâ³ðêà, ÷è áóëè ââåäåí³ àäðåñè
    if (validInput) {
        cout << "You have selected addresses with numbers: ";
        for (int index : selectedIndices) {
            cout << (index + 1) << " "; // Âèâîäèìî íîìåðè â 1-³íäåêñàö³¿
        }
        cout << endl;

    }
    else {
        cout << "No addresses were selected." << endl;
    }
}
