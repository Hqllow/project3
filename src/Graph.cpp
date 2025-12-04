//
// Created by DaleT on 12/1/2025.
//

#include "Graph.h"

#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <climits>
#include <queue>

//constructor
Graph::Graph() {
    edge_list = std::vector<Edge>();
}

void Graph::insert(const Edge& edge) {
    edge_list.push_back(edge);
}

//toggles edges in both directions
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

//iterate through edges and check if toggled if a match is found
bool Graph::checkEdge(int to, int from) {
    for (Edge &edge : edge_list) {
        if (edge.getLocation1() == to && edge.getLocation2() == from) {
            if (edge.isOpen()) {
                std::cout << "open" << std::endl;
                return true;
            }
            std::cout << "closed" << std::endl;
            return false;
        }
        if (edge.getLocation1() == from && edge.getLocation2() == to) {
            if (edge.isOpen()) {
                std::cout << "open" << std::endl;
                return true;
            }
            std::cout << "closed" << std::endl;
            return false;
        }
    }
    std::cout << "DNE" << std::endl;
    return false;
}

//performs a slightly modified bfs on the graph, uses edge list
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

        //iterate over edge list instead of an adj list neighbor iteration
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

//performs dijkstra's on the edge-list, iterates through edges to find neighbors
[[nodiscard]] int Graph::shortestPathTime(int start, int end) const {
    if (start == end) {
        return 0;
    }

    std::unordered_map<int, int> distances;
    for (const auto &edge : edge_list) {
        distances[edge.getLocation1()] = INT_MAX;
        distances[edge.getLocation2()] = INT_MAX;
    }

    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<>> pq;

    distances[start] = 0;
    pq.emplace(0, start);

    while (!pq.empty()) {
        std::pair<int, int> top = pq.top();
        pq.pop();

        const int newDistance = top.first;
        int current = top.second;

        if (newDistance > distances[current]) {
            continue;
        }
        if (current == end) {
            return newDistance;
        }

        //iterate through edges and locates neighbors instead of using easy array like an adj list
        for (const auto &edge : edge_list) {
            if (!edge.isOpen()) {
                continue;
            }

            int neighbor = -1;
            if (edge.getLocation1() == current) {
                neighbor = edge.getLocation2();
            }
            else if (edge.getLocation2() == current) {
                neighbor = edge.getLocation1();
            }
            if (neighbor != -1) {
                int updatedDistance = newDistance + edge.getTime();
                if (updatedDistance < distances[neighbor]) {
                    distances[neighbor] = updatedDistance;
                    pq.emplace(updatedDistance, neighbor);
                }
            }
        }
    }

    return -1;
}

[[nodiscard]] int Graph::getStudentZone(const Student& student) const {
    return 20;
}

