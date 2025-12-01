#include "CampusCompass.h"

#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include "Edge.h"
#include "Graph.h"

using namespace std;

CampusCompass::CampusCompass() {
    graph = Graph();
}

bool CampusCompass::ParseCSV(const string &edges_filepath, const string &classes_filepath) {
    fstream edges;
    fstream classes;

    edges.open(edges_filepath, ios::in);
    classes.open(classes_filepath, ios::in);

    vector<string> row;
    string line, element, temp;

    //remove first line with column headers
    getline(edges, line);

    while (edges >> temp)
    {
        row.clear();
        getline(edges, line);

        stringstream s(line);

        while (getline(s, element, ','))
        {
            row.push_back(element);
        }

    Edge toAdd = Edge(stoi(row[0]), stoi(row[1]), row[2], row[3], stoi(row[4]));
        graph.insert(toAdd);
    }
    edges.close();
    classes.close();
    return true;
}

bool CampusCompass::ParseCommand(const string &command) {
    // do whatever regex you need to parse validity
    // hint: return a boolean for validation when testing. For example:
    bool is_valid = true; // replace with your actual validity checking

    return is_valid;
}
