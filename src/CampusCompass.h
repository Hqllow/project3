#pragma once
#include <string>
#include <unordered_set>

#include "Graph.h"
#include "Class.h"

using namespace std;

class CampusCompass {
private:
    Graph graph;
    std::vector<Student> studentList;
    std:: unordered_set<Class> classSet;
public:
    CampusCompass();
    bool ParseCSV(const string &edges_filepath, const string &classes_filepath);
    bool ParseCommand(const string &command);
};
