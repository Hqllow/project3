#pragma once
#include <string>
#include <set>

#include "Graph.h"
#include "Class.h"

using namespace std;

class CampusCompass {
private:
    Graph graph;
    std::set<Student> studentList;
    std::set<Class> classSet;
    std::string stringRepresentation;
public:
    CampusCompass();
    bool ParseCSV(const string &edges_filepath, const string &classes_filepath);
    bool ParseCommand(const string &command);
    std::string getStringRepresentation();
};
