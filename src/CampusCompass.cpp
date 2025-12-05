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

//uses regex to verify 8 numeric characters in sequence
[[nodiscard]] bool CampusCompass::isValidStudentID(const std::string &id) const {
    regex id_pattern("^\\d{8}$");
    if (!regex_match(id, id_pattern)) {
        return false;
    }
    //check for unique ID
    for (const auto& student : studentList) {
        if (id == student.getID()) {
            return false;
        }
    }
    return true;
}

//uses regex to verify name constraints, alpha + spaces
bool isValidName(const string &name) {
    regex name_pattern("^[A-Za-z ]+$");
    if (!regex_match(name, name_pattern)) {
        return false;
    }
    return true;
}

//uses regex to verify 3 uppercase alpha characters followed by 4 numeric characters in sequence
bool isValidClassCode(const std::string &code) {
    regex code_pattern("^[A-Z]{3}\\d{4}$");
    if (!regex_match(code, code_pattern)) {
        return false;
    }
    return true;
}

//constructor
CampusCompass::CampusCompass(): classSet() {
    graph = Graph();
}

bool CampusCompass::ParseCSV(const string &edges_filepath, const string &classes_filepath) {
    //iterate thru csv and get each edge object
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

    //iterate thru csv and stores all class objects in a set
    ifstream classes(classes_filepath);
    if (!classes.is_open()) {
        cerr << "File error classes" << endl;
        return false;
    }
    row.clear();
    line = "";
    element = "";

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
    if (command.empty()) {
        return false;
    }

    //splits command into individual parts/ arguments
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

    //checks first argument of command for which command to run
    if (parts.at(0) == "insert") {
        //isolate name
        size_t firstQuote = command.find('\"');
        size_t secondQuote = command.find('\"', firstQuote + 1);
        if (firstQuote == string::npos || secondQuote == string::npos) {
            cout << "unsuccessful" << endl;
            return false;
        }
        string studentName = command.substr(firstQuote + 1, secondQuote - firstQuote - 1);

        //add remaining arguments into a vector
        stringstream ss(command.substr(secondQuote + 1));
        vector<string> args;
        string remainingArgs;
        while (ss >> remainingArgs) {
            args.push_back(remainingArgs);
        }

        //check for command restraints
        if (args.size() < 3 || !isValidStudentID(args[0]) || !isValidName(studentName)) {
            cout << "unsuccessful" << endl;
            return false;
        }

        //check for int validity
        int residence = 0;
        int n = 0;
        try {
            residence = stoi(args[1]);
            n = stoi(args[2]);
        } catch (...) {
            cout << "unsuccessful" << endl;
            return false;
        }

        //check size again, make sure student has 6 or less classes.
        if (n > 6 || args.size() != static_cast<size_t>(n + 3)) {
            cout << "unsuccessful" << endl;
            return false;
        }

        //iterate thru all class codes inputted and adds
        vector<Class> classesToAdd;
        for (int i = 0; i < n; i++) {
            const string& code = args[3 + i];
            if (!isValidClassCode(code)) {
                cout << "unsuccessful" << endl;
                return false;
            }

            //locates class in classSet, works because < comparator (used in set) only uses classcode
            Class lookupClass(code, 0, "", "");
            auto cit = classSet.find(lookupClass);
            if (cit == classSet.end()) {
                cout << "unsuccessful" << endl;
                return false;
            }
            classesToAdd.push_back(*cit);
        }

        //same lookup strategy
        Student tempStudent("", args[0], 0);
        if (studentList.find(tempStudent) != studentList.end()) {
            cout << "unsuccessful" << endl;
            return false;
        }

        //finally adds student
        Student newStudent(studentName, args[0], residence);
        newStudent.setClasses(classesToAdd);
        studentList.insert(newStudent);

        cout << "successful" << endl;
        return true;
    }


    if (parts.at(0) == "remove") {
        //check constraints
        if (!isValidStudentID(parts[1])) {
            cout << "unsuccessful" << endl;
            return false;
        }

        //same lookup strategy
        Student tempStudent("", parts[1], 0);

        auto sit = studentList.find(tempStudent);
        if (sit != studentList.end()) {
            studentList.erase(sit);
            cout << "successful" << endl;
            return true;
        }
        cout << "unsuccessful" << endl;
        return false;
    }

    if (parts.at(0) == "dropClass") {
        //check constraints
        if (parts.size() < 3 || !isValidStudentID(parts[1]) || !isValidClassCode(parts[2])) {            cout << "unsuccessful" << endl;
            return false;
        }

        //lookup
        Student tempStudent("", parts[1], 0);
        Class tempClass(parts[2], 0, "", "");
        auto sit = studentList.find(tempStudent);
        if (sit != studentList.end()) {
            Student updatedStudent = *sit;
            //deletes student from list, only re-adds if they have remaining classes
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
        //check constraints
        if (parts.size() != 4 || !isValidStudentID(parts.at(1)) || !isValidClassCode(parts.at(2)) || !isValidClassCode(parts.at(3))) {
            cout << "unsuccessful" << endl;
            return false;
        }

        //lookup
        Class classTemp(parts.at(3), 0, "", "");
        auto cit = classSet.find(classTemp);
        if (cit == classSet.end()) {
            cout << "unsuccessful" << endl;
            return false;
        }
        const Class& newClass = *cit;

        //lookup
        Student tempStudent("", parts.at(1), 0);
        auto sit = studentList.find(tempStudent);
        if (sit == studentList.end()) {
            cout << "unsuccessful" << endl;
            return false;
        }
        Student updatedStudent = *sit;
        studentList.erase(sit);

        //iterate thru student's classes and check if they have the class to be replaced or the one its being replaced with
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

        //leave as is, reinsert student
        if (!hasOld || hasNew) {
            studentList.insert(updatedStudent);
            cout << "unsuccessful" << endl;
            return false;
        }

        //replace class if found
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

            //remove class from classes vector of student
            for (int i = 0; i < classes.size(); i++) {
                if (classes[i].getClassCode() == parts.at(1)) {
                    classes.erase(i + classes.begin());
                    hadClass = true;
                    break;
                }
            }
            //if class was there, increment count and determine whether to add back or remove if no classes remain
            if (hadClass) {
                removedCount++;

                if (classes.empty()) {
                    toRemove.push_back(updated);
                }
                else {
                    updated.setClasses(classes);
                    toReinsert.push_back(updated);
                }
            }
        }
        //reinsert or remove student
        for (auto &student : toReinsert) {
            studentList.erase(student);
            studentList.insert(student);
        }
        for (auto &student : toRemove) {
            studentList.erase(student);
        }
        cout << removedCount << endl;
        return true;
    }


    if (parts.at(0) == "toggleEdgesClosure") {
        if (parts.size() < 3) {
            cout << "unsuccessful" << endl;
            return false;
        }

        //check int validity
        int n = 0;
        try {
            n = stoi(parts.at(1));
        } catch (...) {
            cout << "unsuccessful" << endl;
            return false;
        }
        //check size constraint
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

        //check int validity
        int fromID = 0;
        int toID = 0;
        try {
            fromID = stoi(parts.at(1));
            toID = stoi(parts.at(2));
        } catch (...) {
            cout << "unsuccessful" << endl;
            return false;
        }


        if (graph.checkEdge(fromID, toID)) {
            return true;
        }
        return false;
    }

    if (parts.at(0) == "isConnected") {
        if (parts.size() != 3) {
            cout << "unsuccessful" << endl;
            return false;
        }

        //check int validity
        int fromID = 0;
        int toID = 0;
        try {
            fromID = stoi(parts.at(1));
            toID = stoi(parts.at(2));
        } catch (...) {
            cout << "unsuccessful" << endl;
            return false;
        }

        //relies on graph method
        if (graph.isConnected(fromID, toID)) {
            cout << "successful" << endl;
            return true;
        }
        cout << "unsuccessful" << endl;
        return false;
    }

    if (parts.at(0) == "printShortestEdges") {
        if (parts.size() != 2 || !isValidStudentID(parts.at(1))) {
            cout << "unsuccessful" << endl;
            return false;
        }

        Student tempStudent("", parts.at(1), 0);
        auto sit = studentList.find(tempStudent);
        if (sit == studentList.end()) {
            cout << "unsuccessful" << endl;
            return false;
        }
        const Student& student = *sit;

        cout << "Name: " << student.getName() << endl;

        vector<Class> classes = student.getClasses();
        //lexicographical sort as per instructions; lambda because awesome
        sort(classes.begin(), classes.end(),
             [](const Class &a, const Class &b) {
                 return a.getClassCode() < b.getClassCode();
             });

        //relies on graph method, applies for each class the student has
        for (const auto &c : classes) {
            int time = graph.shortestPathTime(student.getResidence(), c.getLocationID());
            cout << c.getClassCode() << " | Total Time: " << time << endl;
        }
        return true;
    }
    if (parts.at(0) == "printStudentZone") {
        if (parts.size() != 2 || !isValidStudentID(parts.at(1))) {
            cout << "unsuccessful" << endl;
            return false;
        }

        Student tempStudent("", parts.at(1), 0);
        auto sit = studentList.find(tempStudent);
        if (sit == studentList.end()) {
            cout << "unsuccessful" << endl;
            return false;
        }
        const Student& student = *sit;

        //relies on graph function
        cout << "Student Zone Cost For " << student.getName() << ": ";
        cout << graph.getStudentZone(student) << endl;
    }

    //EC
    if (parts.at(0) == "verifySchedule") {

    }

    return false;
}
