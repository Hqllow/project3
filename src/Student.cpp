//
// Created by DaleT on 12/1/2025.
//
#include "Student.h"

//constructor
Student::Student(const std::string &name, const std::string &id, const int &residence) {
    this->name = name;
    this->id = id;
    this->residence = residence;
    classes = std::vector<Class>();
}

//getters
[[nodiscard]] std::string Student::getName() const {
    return name;
}
[[nodiscard]] std::string Student::getID() const {
    return id;
}
[[nodiscard]] int Student::getResidence() const {
    return residence;
}
[[nodiscard]] std::vector<Class> Student::getClasses() const {
    return classes;
}

//setters
void Student::setName(const std::string &name) {
    this->name = name;
}
void Student::setID(const std::string &id) {
    this->id = id;
}
void Student::setResidence(const int &residence) {
    this->residence = residence;
}
void Student::setClasses(const std::vector<Class> &classes) {
    this->classes = classes;
}

//class modifiers
bool Student::removeClass(const Class &c) {
    for (size_t i = 0; i < classes.size(); i++) {
        if (classes.at(i).getClassCode() == c.getClassCode()) {
            classes.erase(classes.begin() + i);
            return true;
        }
    }
    return false;
}
void Student::addClass(const Class &c) {
    this->classes.push_back(c);
}

//operator overrides
bool Student::operator<(const Student &other) const {
    return this->id < other.id;
}

bool Student::operator==(const Student &other) const {
    return this->id == other.id;
}