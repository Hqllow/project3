//
// Created by DaleT on 12/1/2025.
//

#ifndef CLASS_H
#define CLASS_H

#include <string>
#include <vector>

#include "Student.h"

class Class {
public:
    Class(const std::string &cl, const int &location, const std::string &start, const std::string &end);

    [[nodiscard]] std::string getClassCode() const;
    [[nodiscard]] int getLocationID() const;
    [[nodiscard]] std::string getStartTime() const;
    [[nodiscard]] std::string getEndTime() const;

    void setClassCode(const std::string &classCode);
    void setLocationID(const int &locationID);
    void setStartTime(const std::string &startTime);
    void setEndTime(const std::string &endTime);

    bool operator<(const Class &other) const;
    bool operator==(const Class &other) const;

private:
    std::string classCode;
    int locationID;
    std::string startTime;
    std::string endTime;
};
#endif //CLASS_H
