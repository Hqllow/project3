//
// Created by DaleT on 12/1/2025.
//

#include "Graph.h"

Graph::Graph() {
    edge_list = std::priority_queue<Edge>();
}

void Graph::insert(Edge edge)
{
    edge_list.push(edge);
    edge_list.push(edge);
}