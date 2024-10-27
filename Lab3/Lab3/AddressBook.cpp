#include "AddressBook.h"
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <sstream>

AddressBook::AddressBook(string FileName) : filename(FileName) {}

void AddressBook::addAddress(Address* address) {
    addresses.push_back(address);
    // ³�������� ���� ��� ���������
    ofstream file(filename, ios::app); // ³�������� ���� � ����� "������"
    if (file.is_open()) {
        // �������� ������ � ����
        cout << endl;
        file << address->toString() << endl; // ����� � ����
        file.close(); // ��������� ����
    }
    else {
        cerr << "Error: " << filename << endl;
    }
}

void AddressBook::loadAddressesFromFile() {
    ifstream file(filename); // ³�������� ����
    if (!file.is_open()) { // �������� �� ��������
        cerr << "Error: " << filename << endl;
        return; // ����� � �������, ���� ���� �� �������
    }
    string line;
    // ������ ���� ����� �� ������
    while (getline(file, line)) {
        cout << line << endl; // �������� ����� �� �����
    }

    file.close(); // ��������� ����
}

void AddressBook::analyzeAddresses() {

    int privateHouseCount = 0;
    int appartmentCount = 0;
    int buildingCount = 0;
    map<int, int> appartmentDistribution; // ���� ��� �������� ������� �� ���������

    // �������� ���� ������ ������
    for (int index : selectedIndices) {
        if (index >= 0 && index < addresses.size()) { // �������� ���������� �������
            const Address* address = addresses[index];
            if (address->GetType() == "Private House") {
                privateHouseCount++;
            }
            else if (address->GetType() == "Appartment") {
                const Appartment* appartment = dynamic_cast<const Appartment*>(address);
                if (appartment) {
                    appartmentCount++;
                    appartmentDistribution[appartment->GetBuildingNumber()]++;
                }
            }
        }
        else {
            cerr << "Invalid index: " << index << endl; // ������� ��������� �������
        }
    }

    // ϳ�������� ������� ������� (��������� �������)
    buildingCount = appartmentDistribution.size();

    // ���� ����������
    cout << "Number of private houses: " << privateHouseCount << endl;
    cout << "Number of appartments: " << appartmentCount << endl;

    // ���������� �������� ������� ������� �� �������
    if (buildingCount > 0) {
        double averageAppartments = static_cast<double>(appartmentCount) / buildingCount;
        cout << "Average number of appartments per building: " << averageAppartments << endl;
    }
    else {
        cout << "No appartments selected." << endl;
    }

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
    // ������ ���� ����� �� ������ �� �������� �� �����
    while (getline(file, line)) {
        cout << index++ << ": " << line << endl; // �������� ������
    }

    file.close();
}

void AddressBook::selectAddressesForMailing() {
    displayAddresses(); // �������� ������ ��� ������

    cout << "Enter the address numbers (separated by a space) that you want to select for the mailing list: ";
    string input;
    getline(cin, input);

    istringstream iss(input);
    int number;

    selectedIndices.clear(); // ������� �������� ������
    bool validInput = false; // ���� ��� ��������, �� ���� ������ ����� ������

    // ������� ������, ������ ������������
    while (iss >> number) {
        // ������ �� selectedIndices, ���� ����� ������
        if (number > 0) {
            selectedIndices.push_back(number - 1); // �������� ������ � ������ 0-����������
            validInput = true;
        }
    }

    // ��������, �� ���� ������ ������
    if (validInput) {
        cout << "You have selected addresses with numbers: ";
        for (int index : selectedIndices) {
            cout << (index + 1) << " "; // �������� ������ � 1-����������
        }
        cout << endl;

    }
    else {
        cout << "No addresses were selected." << endl;
    }
}