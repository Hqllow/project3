//
// Created by DaleT on 12/1/2025.
//

#include "Graph.h"

#include <iostream>

Graph::Graph() {
    edge_list = std::vector<Edge>();
}

void Graph::insert(const Edge& edge) {
    edge_list.push_back(edge);
}

bool Graph::toggleEdge(int to, int from) {
    bool done = false;
    for (Edge &edge : edge_list) {
        if (edge.getLocation1() == to && edge.getLocation2() == from) {
            edge.toggle();
            done = true;
        }
        if (edge.getLocation1() == from && edge.getLocation2() == to) {
            edge.toggle();
            done = true;
        }
    }
    return done;
}

void Graph::checkEdge(int to, int from) {
    bool done = false;
    for (Edge &edge : edge_list) {
        if (edge.getLocation1() == to && edge.getLocation2() == from) {
            done = true;
            if (edge.isOpen()) {
                std::cout << "open" << std::endl;
            }
            else {
                std::cout << "closed" << std::endl;
            }
        }
        if (edge.getLocation1() == from && edge.getLocation2() == to) {
            done = true;
            if (edge.isOpen()) {
                std::cout << "open" << std::endl;
            }
            else {
                std::cout << "closed" << std::endl;
            }
        }
    }
    if (!done) {
        std::cout << "DNE" << std::endl;
    }
}
