//
// Created by DaleT on 12/1/2025.
//

#ifndef STUDENT_H
#define STUDENT_H

#include "Class.h"
#include <vector>

class Class;

class Student {
public:
    Student(const std::string &name, const std::string &id, const int &residence);

    [[nodiscard]] std::string getName() const;
    [[nodiscard]] std::string getID() const;
    [[nodiscard]] int getResidence() const;
    [[nodiscard]] std::vector<Class> getClasses() const;

    void setName(const std::string &name);
    void setID(const std::string &id);
    void setResidence(const int &residence);
    void setClasses(const std::vector<Class> &classes);

    bool removeClass(const Class &c);
    void addClass(const Class &c);

    bool operator<(const Student &other) const;
    bool operator==(const Student &other) const;

private:
    std::string name;
    std::string id;
    int residence;
    std::vector<Class> classes;
};
#endif //STUDENT_H
