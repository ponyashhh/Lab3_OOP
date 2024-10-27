#include "AddressBook.h"
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <sstream>

AddressBook::AddressBook(string FileName) : filename(FileName) {}

void AddressBook::addAddress(Address* address) {
    addresses.push_back(address);
    // Відкриваємо файл для додавання
    ofstream file(filename, ios::app); // Відкриваємо файл в режимі "додати"
    if (file.is_open()) {
        // Записуємо адресу у файл
        cout << endl;
        file << address->toString() << endl; // Запис у файл
        file.close(); // Закриваємо файл
    }
    else {
        cerr << "Error: " << filename << endl;
    }
}

void AddressBook::loadAddressesFromFile() {
    ifstream file(filename); // Відкриваємо файл
    if (!file.is_open()) { // Перевірка на відкриття
        cerr << "Error: " << filename << endl;
        return; // Вихід з функції, якщо файл не відкрито
    }
    string line;
    // Читаємо файл рядок за рядком
    while (getline(file, line)) {
        cout << line << endl; // Виводимо рядок на екран
    }

    file.close(); // Закриваємо файл
}

void AddressBook::analyzeAddresses() {

    int privateHouseCount = 0;
    int appartmentCount = 0;
    int buildingCount = 0;
    map<int, int> appartmentDistribution; // Мапа для розподілу квартир за будинками

    // Аналізуємо лише вибрані адреси
    for (int index : selectedIndices) {
        if (index >= 0 && index < addresses.size()) { // Перевірка коректності індексу
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
            cerr << "Invalid index: " << index << endl; // Обробка недійсного індексу
        }
    }

    // Підрахунок кількості будинків (унікальних будинків)
    buildingCount = appartmentDistribution.size();

    // Вивід результатів
    cout << "Number of private houses: " << privateHouseCount << endl;
    cout << "Number of appartments: " << appartmentCount << endl;

    // Обчислення середньої кількості квартир на будинок
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
    // Читаємо файл рядок за рядком та виводимо на екран
    while (getline(file, line)) {
        cout << index++ << ": " << line << endl; // Нумеруємо адреси
    }

    file.close();
}

void AddressBook::selectAddressesForMailing() {
    displayAddresses(); // Виводимо адреси для вибору

    cout << "Enter the address numbers (separated by a space) that you want to select for the mailing list: ";
    string input;
    getline(cin, input);

    istringstream iss(input);
    int number;

    selectedIndices.clear(); // Очищуємо попередні вибори
    bool validInput = false; // Флаг для перевірки, чи були введені валідні номери

    // Зчитуємо номери, введені користувачем
    while (iss >> number) {
        // Додаємо до selectedIndices, якщо номер дійсний
        if (number > 0) {
            selectedIndices.push_back(number - 1); // Зберігаємо індекс у форматі 0-індексації
            validInput = true;
        }
    }

    // Перевірка, чи були введені адреси
    if (validInput) {
        cout << "You have selected addresses with numbers: ";
        for (int index : selectedIndices) {
            cout << (index + 1) << " "; // Виводимо номери в 1-індексації
        }
        cout << endl;

    }
    else {
        cout << "No addresses were selected." << endl;
    }
}