//
// Created by DaleT on 12/1/2025.
//

#ifndef GRAPH_H
#define GRAPH_H
#include <unordered_map>
#include <vector>
#include <Edge.h>

class Graph {
public:
    Graph();
    void insert(Edge edge);

private:
    std::vector<Edge> edge_list;
};
#endif //GRAPH_H
