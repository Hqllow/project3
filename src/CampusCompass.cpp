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

bool isValidClassCode(const std::string &code) {
    regex id_pattern("^[A-Z]{3}\\d{4}$");
    if (!regex_match(code, id_pattern)) {
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
        size_t firstQuote = command.find('\"');
        size_t secondQuote = command.find('\"', firstQuote + 1);
        if (firstQuote == string::npos || secondQuote == string::npos) {
            cout << "unsuccessful" << endl;
            return false;
        }
        string studentName = command.substr(firstQuote + 1, secondQuote - firstQuote - 1);
        stringstream ss(command.substr(secondQuote + 1));
        vector<string> args;
        string element;
        while (ss >> element) {
            args.push_back(element);
        }

        if (args.size() < 3 || !isValidStudentID(args[0]) || studentName.empty()) {
            cout << "unsuccessful" << endl;
            return false;
        }

        int residence = 0;
        int n = 0;
        try {
            residence = stoi(args[1]);
            n = stoi(args[2]);
        } catch (...) {
            cout << "unsuccessful" << endl;
            return false;
        }

        if (args.size() != static_cast<size_t>(n + 3)) {
            cout << "unsuccessful" << endl;
            return false;
        }

        vector<Class> classesToAdd;
        for (int i = 0; i < n; i++) {
            string code = args[3 + i];
            if (!isValidClassCode(code)) {
                cout << "unsuccessful" << endl;
                return false;
            }

            Class lookupClass(code, 0, "", "");
            auto classIt = classSet.find(lookupClass);
            if (classIt == classSet.end()) {
                cout << "unsuccessful" << endl;
                return false;
            }
            classesToAdd.push_back(*classIt);
        }

        Student tempStudent("", args[0], 0);
        if (studentList.find(tempStudent) != studentList.end()) {
            cout << "unsuccessful" << endl;
            return false;
        }

        Student newStudent(studentName, args[0], residence);
        newStudent.setClasses(classesToAdd);
        studentList.insert(newStudent);

        cout << "successful" << endl;
        return true;
    }


    if (parts.at(0) == "remove") {
        if (!isValidStudentID(parts[1])) {
            cout << "unsuccessful" << endl;
            return false;
        }

        Student tempStudent("", parts[1], 0);

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
        if (parts.size() < 3 || !isValidStudentID(parts[1]) || !isValidClassCode(parts[2])) {            cout << "unsuccessful" << endl;
            return false;
        }

        Student tempStudent("", parts[1], 0);
        Class tempClass(parts[2], 0, "", "");
        auto sit = studentList.find(tempStudent);
        if (sit != studentList.end()) {
            Student updatedStudent = *sit;
            studentList.erase(sit);
            if (updatedStudent.removeClass(tempClass)) {
                if (updatedStudent.getClasses().empty()) {
                    cout << "successful" << endl;
                    return true;
                }
                studentList.insert(updatedStudent);
                cout << "successful" << endl;
                return true;
            }
            studentList.insert(updatedStudent);
        }
        cout << "unsuccessful" << endl;
        return false;
    }
    if (parts.at(0) == "replaceClass") {
        if (parts.size() != 4 || !isValidStudentID(parts.at(1)) || !isValidClassCode(parts.at(2)) || !isValidClassCode(parts.at(3))) {
            cout << "unsuccessful" << endl;
            return false;
        }

        Class classTemp(parts.at(3), 0, "", "");
        auto cit = classSet.find(classTemp);
        if (cit == classSet.end()) {
            cout << "unsuccessful" << endl;
            return false;
        }
        Class newClass = *cit;

        Student tempStudent("", parts.at(1), 0);
        auto sit = studentList.find(tempStudent);
        if (sit == studentList.end()) {
            cout << "unsuccessful" << endl;
            return false;
        }
        Student updatedStudent = *sit;
        studentList.erase(sit);

        vector<Class> currentClasses = updatedStudent.getClasses();
        bool hasOld = false;
        bool hasNew = false;

        for (const auto &c : currentClasses) {
            if (c.getClassCode() == parts.at(2)) {
                hasOld = true;
            }
            if (c.getClassCode() == parts.at(3)) {
                hasNew = true;
            }
        }

        if (!hasOld || hasNew) {
            studentList.insert(updatedStudent);
            cout << "unsuccessful" << endl;
            return false;
        }

        for (auto & currentClass : currentClasses) {
            if (currentClass.getClassCode() == parts.at(2)) {
                currentClass = newClass;
                break;
            }
        }
        updatedStudent.setClasses(currentClasses);
        studentList.insert(updatedStudent);
        cout << "successful" << endl;
        return true;
    }
    if (parts.at(0) == "removeClass") {
        if (parts.size() != 2 || !isValidClassCode(parts.at(1))) {
            cout << "unsuccessful" << endl;
            return false;
        }
        Class tempClass(parts.at(1), 0, "", "");
        auto cit = classSet.find(tempClass);
        if (cit == classSet.end()) {
            cout << "0" << endl;
            return true;
        }

        int removedCount = 0;
        vector<Student> toReinsert;
        vector<Student> toRemove;

        for (auto updated : studentList) {
            vector<Class> classes = updated.getClasses();
            bool hadClass = false;

            for (size_t i = 0; i < classes.size(); i++) {
                if (classes[i].getClassCode() == parts.at(1)) {
                    classes.erase(classes.begin() + i);
                    hadClass = true;
                    break;
                }
            }
            if (hadClass) {
                removedCount++;

                if (classes.empty()) {
                    toRemove.push_back(updated);
                } else {
                    updated.setClasses(classes);
                    toReinsert.push_back(updated);
                }
            }
        }
        for (auto &s : toReinsert) {
            studentList.erase(s);
            studentList.insert(s);
        }
        for (auto &s : toRemove) {
            studentList.erase(s);
        }
        cout << removedCount << endl;
        return true;
    }
    if (parts.at(0) == "toggleEdgesClosure") {
        if (parts.size() < 3) {
            cout << "unsuccessful" << endl;
            return false;
        }

        int n = 0;
        try {
            n = stoi(parts.at(1));
        } catch (...) {
            cout << "unsuccessful" << endl;
            return false;
        }
        if (parts.size() != static_cast<size_t>(2 * n + 2)) {
            cout << "unsuccessful" << endl;
            return false;
        }

        for (int i = 0; i < n; i++) {
            int fromID = stoi(parts.at(2 + 2 * i));
            int toID = stoi(parts.at(3 + 2 * i));

            graph.toggleEdge(fromID, toID);
        }

        cout << "successful" << endl;
        return true;
    }
    if (parts.at(0) == "checkEdgeStatus") {
        if (parts.size() < 3) {
            cout << "unsuccessful" << endl;
            return false;
        }

        int fromID = 0;
        int toID = 0;
        try {
            fromID = stoi(parts.at(1));
            toID = stoi(parts.at(2));
        } catch (...) {
            cout << "unsuccessful" << endl;
            return false;
        }

        graph.checkEdge(fromID, toID);
        return true;
    }
    if (parts.at(0) == "isConnected") {
        if (parts.size() != 3) {
            cout << "unsuccessful" << endl;
            return false;
        }

        int fromID = 0;
        int toID = 0;
        try {
            fromID = stoi(parts.at(1));
            toID = stoi(parts.at(2));
        } catch (...) {
            cout << "unsuccessful" << endl;
            return false;
        }

        if (graph.isConnected(fromID, toID)) {
            cout << "successful" << endl;
        }
        else {
            cout << "unsuccessful" << endl;
        }

        return true;
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
