#pragma once
#include <string>

#include "Graph.h"

using namespace std;

class CampusCompass {
private:
    Graph graph;

public:
    CampusCompass();
    bool ParseCSV(const string &edges_filepath, const string &classes_filepath);
    bool ParseCommand(const string &command);
};
