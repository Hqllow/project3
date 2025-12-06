//
// Created by DaleT on 12/1/2025.
//

#include "Graph.h"

#include <algorithm>
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

[[nodiscard]] int Graph::getStudentZone(const Student &student) const {
    int start = student.getResidence();
    std::vector<Class> classes = student.getClasses();

    //collect all class locations and put in vector
    std::vector<int> vertices;
    vertices.push_back(start);
    for (const auto &c : classes) {
        vertices.push_back(c.getLocationID());
    }

    //puts only unique locations in vertices, as some classes share the same location
    std::vector<int> uniqueVertices;
    for (size_t i = 0; i < vertices.size(); ++i) {
        bool alreadyExists = false;
        for (size_t j = 0; j < uniqueVertices.size(); ++j) {
            if (vertices[i] == uniqueVertices[j]) {
                alreadyExists = true;
                break;
            }
        }
        if (!alreadyExists) {
            uniqueVertices.push_back(vertices[i]);
        }
    }

    vertices = uniqueVertices;

    //compiles all edges into a list so we can do kruskal's, stored as location1, location2, distance
    std::vector<std::tuple<int,int,int>> edges;

    //create subgraph stage, compiles all into edges vector if a path exists, check for path using dijkstra's function from earlier
    for (size_t i = 0; i < vertices.size(); i++) {
        for (size_t j = i + 1; j < vertices.size(); j++) {
            int u = vertices[i];
            int v = vertices[j];
            int dist = shortestPathTime(u, v);
            if (dist != -1) {
                edges.emplace_back(u, v, dist);
            }
        }
    }

    //no connections check (bad)
    if (edges.empty()) {
        return -1;
    }

    //set each vertex to own parent
    std::unordered_map<int, int> parent;
    for (int v : vertices) {
        parent[v] = v;
    }

    //lambda because awesome
    auto find = [&](int x) {
        while (parent[x] != x) {
            parent[x] = parent[parent[x]];
            x = parent[x];
        }
        return x;
    };

    //lambda because awesome #2 electric boogaloo
    auto unite = [&](int a, int b) {
        int parentA = find(a), parentB = find(b);
        if (parentA != parentB) parent[parentB] = parentA;
    };

    //lambda because awesome x3
    //sort with inline lambda comparison, sets up kruskals as we iterate over sorted vector in which smallest edges are first
    std::sort(edges.begin(), edges.end(),
              [](const auto &a, const auto &b) {
                  return std::get<2>(a) < std::get<2>(b);
              });

    //combine edge disjoint sets for all edges, (kruskal's since smallest first)
    int totalCost = 0;
    int edgeCount = 0;
    for (const auto &[u, v, w] : edges) {
        //cycle check
        if (find(u) != find(v)) {
            unite(u, v);
            totalCost += w;
            edgeCount++;
        }
    }

    //check for valid MST, should always be n - 1 edges
    if (edgeCount == static_cast<int>(vertices.size()) - 1) {
        return totalCost;
    }
    return -1;
}