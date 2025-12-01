//
// Created by DaleT on 12/1/2025.
//
#include "Student.h"

Student::Student(const std::string &name, const int &id, const int &residence) {
    this->name = name;
    this->id = id;
    this->residence = residence;
}

[[nodiscard]] std::string Student::getName() const {
    return name;
}
[[nodiscard]] int Student::getID() const {
    return id;
}
[[nodiscard]] int Student::getResidence() const {
    return residence;
}
[[nodiscard]] std::vector<Class> Student::getClasses() const {
    return classes;
}

void Student::setName(const std::string &name) {
    this->name = name;
}
void Student::setID(const int &id) {
    this->id = id;
}
void Student::setResidence(const int &residence) {
    this->residence = residence;
}
void Student::setClasses(const std::vector<Class> &classes) {
    this->classes = classes;
}

void Student::removeClass(const Class &c) {
    for (size_t i = 0; i < classes.size(); i++) {
        if (classes.at(i).getLocationID() == c.getLocationID() && classes.at(i).getClassCode() == c.getClassCode()) {
            classes.erase(classes.begin() + i);
        }
    }
}
void Student::addClass(const Class &c) {
    this->classes.push_back(c);
}