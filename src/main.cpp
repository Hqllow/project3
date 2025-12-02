#include <iostream>

#include "CampusCompass.h"

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    // initialize your main project object
    CampusCompass compass;

    // ingest CSV data
    compass.ParseCSV("data/edges.csv", "data/classes.csv");

    // the below is example code for parsing commandline input
    int no_of_lines;
    string command;
    cin >> no_of_lines;
    cin.ignore(); // ignore newline that first cin left over
    for (int i = 0; i < no_of_lines; i++) {
        getline(cin, command);

        if (command.empty()) {
            i--;
            continue;
        }

        // parse your commands however you see fit
        compass.ParseCommand(command);
        cout.flush();
    }
    cout.flush();
    return 0;
}
