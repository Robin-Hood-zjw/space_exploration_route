//Name: Jiawen Zhang; Lab: 011
//Date: 04/10/2022
//This is the main file called planPlanet.cpp, and this project is implemented by individual. All the
//required algorithms and data  structures are written in this file.
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
using namespace std;

//create a struct to receive the info from the location input stream
struct locVect {
    int row;
    int col;
    string symbol;
    int id;
};

//create a struct to receive the info from the names input stream
struct nameVect {
    int id;
    string name;
};

//create a struct to combine the info from two streams
struct Planet {
    int row;
    int col;
    string symbol;
    int id;
    string name;
    string status;
};

//


int main() {
    // 1. Reads in the locations and planet names

    //get file names from a user's input
    //initialize the variables for users' input
    string locationFile;
    string nameFile;
    cout << "Enter Locations Filename: ";
    cin >> locationFile;
    cout << "Enter Names Filename: ";
    cin >> nameFile;

    //open the target files
    //initialize the input file streams for the location file and the name file
    ifstream fin(locationFile);
    ifstream fin2(nameFile);
    if ((!fin.is_open()) || (!fin2.is_open())) {
        cout << "“Input file could not be opened" << endl;
        return 1;
    }

    //read data from two input file streams
    //read the first six parameters from the location file
    int num;//the variable for the first six integer input from the location file
    int gridRow;//the max row index number of the target grid area
    int gridCol;//the max column index number of the target grid area
    int startRow;
    int startCol;
    int endRow;
    int endCol;
    int idx = 0;
    //abstract the first six element from the location input stream
    while (fin >> num) {
        switch (idx) {
            case 0:
                gridRow = num;
            case 1:
                gridCol = num;
            case 2:
                startRow = num;
            case 3:
                startCol = num;
            case 4:
                endRow = num;
            case 5:
                endCol = num;
        }
        if ((++idx) >= 6) {
            break;
        } else {
            continue;
        }
    }
    //save the data retrieved from a location input stream into a vector of locations
    vector<locVect> locVector;
    locVect location;
    while (fin >> location.row >> location.col >> location.symbol >> location.id) {
        if ((location.row >= 1) && (location.row <= gridRow) && (location.col >= 1) && (location.col <= gridCol)) {
            locVector.push_back(location);
        } else {
            cout << location.id << " out of range - ignoring" << endl;
        }
    }

    //save the data retrieved from a name input stream into a vector of names
    vector<nameVect> nameVector;
    nameVect nameInfo;
    while (fin2 >> nameInfo.id >> nameInfo.name) {
        nameVector.push_back(nameInfo);
    }

    //close all the input file streams
    fin.close();
    fin2.close();

    //combine structs via the same id
    vector<Planet> planets;
    Planet planet;
    for (int i=0; i<int(locVector.size()); i++) {
        for (int j=0; j<int(nameVector.size()); j++) {
            if (locVector[i].id == nameVector[j].id) {
                planet = {locVector[i].row, locVector[i].col, locVector[i].symbol, locVector[i].id, nameVector[j].name, "unvisited"};
                planets.push_back(planet);
            } else {
                continue;
            }
        }
    }


    // 2. Processes the locations and planet names
    //Correct Errors
    for (int i = 0; i < int(planets.size()); ++i) {
        while ((planets[i].name.find("XX") != string::npos) || (planets[i].name.find("_") != string::npos)) {
            if (planets[i].name.find("XX") != string::npos) {
                int xxPos = planets[i].name.find("XX");
                planets[i].name.replace(xxPos,2,"");
            } else if (planets[i].name.find("_") != string::npos) {
                int underscorePos = planets[i].name.find("_");
                planets[i].name.replace(underscorePos,1," ");
            }
        }
    }



    // 3. Determines the route to take
    //keep searching the shortest path for the current position
    double dist = 2147483647;
    int loopIdx = 0;
    int targetIdx;
    int nextRow;
    int nextCol;
    int currentRow = startRow;//the row index of the
    int currentCol = startCol;

    ofstream fileOut("journey.txt");
    for (int rowIdx = 1; rowIdx <= gridRow; ++rowIdx) {
        for (int colIdx = 1; colIdx <= gridCol; ++colIdx) {
            bool test = true;
            if (rowIdx == startRow && colIdx == startCol) {
                fileOut << "S";
                test = false;
            } else if (rowIdx == endRow && colIdx == endCol) {
                fileOut << "E";
                test = false;
            } else {
                int vecIdx = 0;
                while (vecIdx < int(planets.size())){
                    if (rowIdx == planets[vecIdx].row && colIdx == planets[vecIdx].col) {
                        fileOut << planets[vecIdx].symbol;
                        test = false;
                    }
                    vecIdx++;
                }
                if (test) {
                    fileOut << ".";
                }
            }
        }
        fileOut << endl;
    }

    fileOut << "Start at " << startRow << " " << currentCol << endl;
    while (loopIdx < int(planets.size())) {//execute enough times to find each time's target planet

        for (int i = 0; i < int(planets.size()); ++i) {
            double tempVal = sqrt(pow((planets[i].row - currentRow),2) + pow((planets[i].col - currentCol),2));
            if ((planets[i].status == "unvisited") && (tempVal < dist)) {
                targetIdx = i;
                dist = tempVal;
            }
        }
        planets[targetIdx].status = "visited";
        fileOut << "Go to " << planets[targetIdx].name << " at " << planets[targetIdx].row << " " << planets[targetIdx].col << endl;
        currentRow = planets[targetIdx].row;
        currentCol = planets[targetIdx].col;
        dist = 2147483647;
        loopIdx++;
    }
    fileOut << "End at " << endRow << " " << endCol << endl;
    fileOut.close();
    return 0;
}