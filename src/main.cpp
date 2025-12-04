#include <iostream>

#include "CampusCompass.h"

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    CampusCompass compass;

    compass.ParseCSV("data/edges.csv", "data/classes.csv");

    //reads commands line by line
    int no_of_lines;
    string command;
    cin >> no_of_lines;
    cin.ignore();
    for (int i = 0; i < no_of_lines; i++) {
        getline(cin, command);

        if (command.empty()) {
            i--;
            continue;
        }

        //pass command to object for parsing
        compass.ParseCommand(command);
        cout.flush();
    }
    cout.flush();
    return 0;
}
