//
// Created by DaleT on 12/1/2025.
//

#include <string>
#include "Edge.h"

    Edge::Edge(const bool &active, const int &id1, const int &id2, const std::string &n1, const std::string &n2, const int &t) {
        isActive = active;
        locationID_1 = id1;
        locationID_2 = id2;
        name_1 = n1;
        name_2 = n2;
        time = t;
    }

    [[nodiscard]] bool Edge::isOpen() const {
        return isActive;
    }

    [[nodiscard]] int Edge::getLocation1() const {
        return locationID_1;
    }

    [[nodiscard]] int Edge::getLocation2() const {
        return locationID_2;
    }

    [[nodiscard]] std::string Edge::getName1() const {
        return name_1;
    }

    [[nodiscard]] std::string Edge::getName2() const {
        return name_2;
    }

    [[nodiscard]] int Edge::getTime() const {
        return time;
    }

    void Edge::setOpen(const bool &active) {
        isActive = active;
    }

    void Edge::setLocation1(const int &id) {
        locationID_1 = id;
    }

    void Edge::setLocation2(const int &id) {
        locationID_2 = id;
    }

    void Edge::setName1(const std::string &name) {
        name_1 = name;
    }

    void Edge::setName2(const std::string &name) {
        name_2 = name;
    }

    void Edge::setTime(const int &t) {
        time = t;
    }