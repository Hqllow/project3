//
// Created by DaleT on 12/1/2025.
//
#include "Class.h"
#include <string>

//constructor
Class::Class(const std::string &cl, const int &location, const std::string &start, const std::string &end) {
    classCode = cl;
    locationID = location;
    startTime = start;
    endTime = end;
}

//getters
[[nodiscard]] std::string Class::getClassCode() const {
    return classCode;
}
[[nodiscard]] int Class::getLocationID() const {
    return locationID;
}
[[nodiscard]] std::string Class::getStartTime() const {
    return startTime;
}
[[nodiscard]] std::string Class::getEndTime() const {
    return endTime;
}

//setters
void Class::setClassCode(const std::string &classCode) {
    this->classCode = classCode;
}
void Class::setLocationID(const int &locationID) {
    this->locationID = locationID;
}
void Class::setStartTime(const std::string &startTime) {
    this->startTime = startTime;
}
void Class::setEndTime(const std::string &endTime) {
    this->endTime = endTime;
}

//operator overrides
bool Class::operator<(const Class &other) const {
    return classCode < other.getClassCode();
}
bool Class::operator==(const Class &other) const {
    return classCode == other.getClassCode();
}