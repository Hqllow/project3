//
// Created by DaleT on 12/1/2025.
//

#ifndef EDGE_H
#define EDGE_H
#include <string>

class Edge {
public:
    Edge(const bool &active, const int &id1, const int &id2, const std::string &n1, const std::string &n2, const int &t);

    [[nodiscard]] bool isOpen() const;
    [[nodiscard]] int getLocation1() const;
    [[nodiscard]] int getLocation2() const;
    [[nodiscard]] std::string getName1() const;
    [[nodiscard]] std::string getName2() const;
    [[nodiscard]] int getTime() const;

    void setOpen(const bool &active);
    void setLocation1(const int &id);
    void setLocation2(const int &id);
    void setName1(const std::string &name);
    void setName2(const std::string &name);
    void setTime(const int &t);

    bool operator<(const Edge &other) const;
private:
    bool isActive;
    int locationID_1;
    int locationID_2;
    std::string name_1;
    std::string name_2;
    int time;
};
#endif //EDGE_H
