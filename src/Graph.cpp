//
// Created by DaleT on 12/1/2025.
//

#include "Graph.h"

#include <iostream>
#include <unordered_set>
#include <queue>

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

[[nodiscard]] bool Graph::isConnected(int from, int to) const {
    if (from == to) {
        return true;
    }

    std::queue<int> q;
    std::unordered_set<int> visited;

    q.push(from);
    visited.insert(from);

    while (!q.empty()) {
        int curr = q.front();
        q.pop();

        if (curr == to) {
            return true;
        }

        for (const auto &edge : edge_list) {
            if (!edge.isOpen()) continue;

            int neighbor = -1;
            if (edge.getLocation1() == curr) {
                neighbor = edge.getLocation2();
            }
            else if (edge.getLocation2() == curr) {
                neighbor = edge.getLocation1();
            }
            if (neighbor != -1 && !visited.count(neighbor)) {
                visited.insert(neighbor);
                q.push(neighbor);
            }
        }
    }
    return false;
}
