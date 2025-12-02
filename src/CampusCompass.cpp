#include "CampusCompass.h"

#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <regex>
#include "Edge.h"
#include "Graph.h"

using namespace std;

bool isValidStudentID(const std::string &id) {
    regex id_pattern("^\\d{8}$");
    if (!regex_match(id, id_pattern)) {
        return false;
    }
    return true;
}

CampusCompass::CampusCompass(): classSet() {
    graph = Graph();
}

bool CampusCompass::ParseCSV(const string &edges_filepath, const string &classes_filepath) {
    ifstream edges(edges_filepath);

    if (!edges.is_open()) {
        cerr << "File error edges" << endl;
        return false;
    }

    vector<string> row;
    string line, element, temp;

    //remove first line with column headers
    getline(edges, line);

    while (getline(edges, line))
    {
        if (line.empty()) {
            continue;
        }
        row.clear();
        stringstream s(line);

        while (getline(s, element, ','))
        {
            row.push_back(element);
        }
        Edge toAdd = Edge(true, stoi(row[0]), stoi(row[1]), row[2], row[3], stoi(row[4]));
        graph.insert(toAdd);
    }
    edges.close();

    ifstream classes(classes_filepath);
    if (!classes.is_open()) {
        cerr << "File error classes" << endl;
        return false;
    }
    row.clear();
    line = "";
    element = "";
    temp = "";

    //remove first line with column headers
    getline(classes, line);

    while (getline(classes, line))
    {
        if (line.empty()) {
            continue;
        }
        row.clear();
        stringstream s(line);

        while (getline(s, element, ','))
        {
            row.push_back(element);
        }
        Class toAdd = Class(row[0], stoi(row[1]), row[2], row[3]);
        classSet.insert(toAdd);
    }
    classes.close();
    return true;
}

bool CampusCompass::ParseCommand(const string &command) {
    // do whatever regex you need to parse validity
    // hint: return a boolean for validation when testing. For example:
    bool is_valid = true; // replace with your actual validity checking

    if (command.empty()) {
        return false;
    }

    vector<string> parts;
    string element;
    stringstream s(command);

    while (getline(s, element, ' ')) {
        //skip extra spaces
        if (!element.empty()) {
            parts.push_back(element);
        }
    }

    if (parts.size() < 2) {
        cout << "unsuccessful" << endl;
        return false;
    }

    if (parts.at(0) == "insert") {

    }
    if (parts.at(0) == "remove") {
        const string& studentID = parts[1];

        if (isValidStudentID(studentID)) {
            cout << "unsuccessful" << endl;
            return false;
        }

        int stuID = stoi(studentID);

        Student tempStudent("", stuID, 0);

        auto it = studentList.find(tempStudent);
        if (it != studentList.end()) {
            studentList.erase(it);
            cout << "successful" << endl;
            return true;
        } else {
            cout << "unsuccessful" << endl;
            return false;
        }
    }
    if (parts.at(0) == "dropClass") {

    }
    if (parts.at(0) == "replaceClass") {

    }
    if (parts.at(0) == "removeClass") {

    }
    if (parts.at(0) == "toggleEdgesClosure") {

    }
    if (parts.at(0) == "checkEdgeStatus") {

    }
    if (parts.at(0) == "isConnected") {

    }
    if (parts.at(0) == "printShortestEdges") {

    }
    if (parts.at(0) == "printStudentZone") {

    }

    //EC
    if (parts.at(0) == "verifySchedule") {

    }

    return is_valid;
}
