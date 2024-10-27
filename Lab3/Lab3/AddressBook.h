#pragma once
#ifndef ADDRESSBOOK_H 
#define ADDRESSBOOK_H

#include <vector>
#include "Address.h"
#include "PrivateHouse.h"
#include "Appartment.h"


class AddressBook {
private:
    vector<Address*> addresses;
    string filename;
    vector<int> selectedIndices; // Вектор для зберігання обраних адрес
public:
    AddressBook(string FileName);
    void addAddress(Address* address);
    void loadAddressesFromFile();
    void analyzeAddresses();
    const vector<Address*>& getAddresses() const;
    ~AddressBook();

    void displayAddresses(); 
    void selectAddressesForMailing(); 

};

#endif // ADDRESSBOOK_H