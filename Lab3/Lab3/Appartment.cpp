#include "Appartment.h"

Appartment::Appartment(string City, string Street, int BuildingNumber, int AppartmentNumber)
    : Address(City, Street, BuildingNumber), appartmentNumber(AppartmentNumber) {}

int Appartment::GetAppartmentNumber() const {
    return appartmentNumber;
}

string Appartment::GetType() const {
    return "Appartment";
}
string Appartment::toString() const {
    return "Appartment Building: " + city + " " + street + " " + std::to_string(buildingNumber) + " " + std::to_string(appartmentNumber);
}