//
// Created by DaleT on 12/1/2025.
//

#ifndef GRAPH_H
#define GRAPH_H
#include <vector>
#include "Student.h"
#include "Edge.h"

class Graph {
public:
    Graph();
    void insert(const Edge& edge);
    bool toggleEdge(int to, int from);
    bool checkEdge(int to, int from);
    [[nodiscard]] bool isConnected(int from, int to) const;
    [[nodiscard]] int shortestPathTime(int start, int end) const;
    [[nodiscard]] int getStudentZone(const Student& student) const;
private:
    std::vector<Edge> edge_list;
};
#endif //GRAPH_H
