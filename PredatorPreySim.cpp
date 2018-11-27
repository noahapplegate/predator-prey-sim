#include "Organism.h"
#include "Ant.h"
#include "Doodlebug.h"
#include "Grid.h"
#include <list>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <ctime>
#include <random>
#include <conio.h>

using namespace std;

//Menu choice
enum class MenuChoice{ NewFile = 1, RunCurrentSim, ReadNextSim, Exit };

//Structure containing information used to generate a simulation
struct OrganismRecord
{
    unsigned short gridWidth;
    unsigned short gridHeight;
    unsigned short numDoodlebugs;
    unsigned short numAnts;
};

//Function prototypes
void openSimFile(string& fileName, ifstream& infile);
void runSim(const OrganismRecord& simInfo);
bool compareOrganisms(const unique_ptr<Organism>& left, const unique_ptr<Organism>& right);
void readSimInfo(ifstream& infile, OrganismRecord& simInfo);
void placeOrganisms(const OrganismRecord& simInfo, Grid& world, list<unique_ptr<Organism>>& orgList);
void performActions(list<unique_ptr<Organism>>::iterator orgIt, list<unique_ptr<Organism>>& orgList);

int main()
{
    string fileName{ "N/A" };
    ifstream infile;
    int tempInt;
    OrganismRecord simInfo;
    MenuChoice userChoice;

    do
    {
        system("CLS");

        cout << "PREDATOR-PREY SIMULATION\n";
        cout << "------------------------\n";
        cout << "1. Open new simulation file\n";
        cout << "2. Run the current simuation\n";
        cout << "3. Read the next simulation in the file\n";
        cout << "4. Exit the program\n";
        cout << "Current simulation file: " << fileName << "\n";
        cout << "Enter choice: ";
        cin >> tempInt;

        userChoice = static_cast<MenuChoice>(tempInt);

        switch (userChoice)
        {
        case MenuChoice::NewFile:
            openSimFile(fileName, infile);
            readSimInfo(infile, simInfo);

            break;

        case MenuChoice::RunCurrentSim:
            if (infile.is_open())
            {
                runSim(simInfo);
            }

            break;

        case MenuChoice::ReadNextSim:
            if (infile.is_open())
            {
                readSimInfo(infile, simInfo);
            }

            break;

        case MenuChoice::Exit:
            cout << "Exiting program.\n";
            infile.close();

            break;

        default:
            cout << "Invalid menu choice\n";
            cout << "Enter a value between 1 and 4\n";

            break;
        }
    } while (userChoice != MenuChoice::Exit);

    return 0;
}

void openSimFile(string& fileName, ifstream& infile)
{
    cout << "Enter the name of the simulation file: ";
    cin >> fileName;

    if (infile.is_open())
    {
        infile.close();
    }

    infile.open(fileName.c_str());

    while (!infile.is_open())
    {
        cout << endl;
        cout << "Error, file not found.\n";
        cout << "Enter the name of the simulation file: ";
        cin >> fileName;
        infile.open(fileName);
    }
}

void runSim(const OrganismRecord& simInfo)
{
    Grid world1(simInfo.gridWidth, simInfo.gridHeight);
    list<unique_ptr<Organism>> orgList;

    placeOrganisms(simInfo, world1, orgList);

    char userChoice{ 'a' };

    do
    {
        //Remove dead organisms from the list
        auto organismIt = orgList.begin();
        while (organismIt != orgList.end())
        {
            //If the organism is dead remove it, otherwise check the next organism
            ((*organismIt)->isDead()) ? (organismIt = orgList.erase(organismIt)) : (++organismIt);
        }

        //Refresh the screen
        system("CLS");
        cout << "Grid Width: " << simInfo.gridWidth << " Grid Height: " << simInfo.gridHeight << endl;
        cout << "Ants remaining: " << Ant::getAntCount() << " Doodlebugs remaining: " << Doodlebug::getDoodlebugCount() << endl;
        cout << endl;
        world1.displayGrid();

        //Sort the organism list
        orgList.sort(compareOrganisms);

        //Perform each organism's actions
        organismIt = orgList.begin();
        while (organismIt != orgList.end())
        {
            performActions(organismIt, orgList);

            ++organismIt;
        }

        cout << endl;
        cout << "Press any key to continue\n";
        cout << "Press 'q' to quit\n";

        userChoice = _getch();
    } while ((userChoice != 'q') && (Ant::getAntCount() != 0) && (Doodlebug::getDoodlebugCount() != 0));

    cout << endl;
    cout << "End of simulation\n";
    cout << "Press any key to continue\n";
    userChoice = _getch();
}

//Return true if left is ordered before right
bool compareOrganisms(const unique_ptr<Organism>& left, const unique_ptr<Organism>& right)
{
    bool test{ (*left) < (*right) };
    return test;
}

void readSimInfo(ifstream& infile, OrganismRecord& simInfo)
{
    string line;
    string tempString;

    getline(infile, line);

    stringstream info{ line };

    getline(info, tempString, ',');
    simInfo.gridWidth = stoi(tempString);

    getline(info, tempString, ',');
    simInfo.gridHeight = stoi(tempString);

    getline(info, tempString, ',');
    simInfo.numDoodlebugs = stoi(tempString);

    getline(info, tempString, '\n');
    simInfo.numAnts = stoi(tempString);
}

void placeOrganisms(const OrganismRecord& simInfo, Grid& world, list<unique_ptr<Organism>>& orgList)
{
    default_random_engine engine{ static_cast<unsigned int>(time(0)) };
    uniform_int_distribution<unsigned int> randomXPos(0, simInfo.gridWidth - 1);
    uniform_int_distribution<unsigned int> randomYPos(0, simInfo.gridHeight - 1);

    for (size_t i{ 0 }; i < simInfo.numDoodlebugs; ++i)
    {
        unsigned int x{ randomXPos(engine) };
        unsigned int y{ randomYPos(engine) };

        while (!(world.availablePosition(x, y)))
        {
            x = randomXPos(engine);
            y = randomYPos(engine);
        }

        orgList.push_back(static_cast<unique_ptr<Organism>>(new Doodlebug(x, y, &world)));
    }

    for (size_t i{ 0 }; i < simInfo.numAnts; ++i)
    {
        unsigned int x{ randomXPos(engine) };
        unsigned int y{ randomYPos(engine) };

        while (!(world.availablePosition(x, y)))
        {
            x = randomXPos(engine);
            y = randomYPos(engine);
        }

        orgList.push_back(static_cast<unique_ptr<Organism>>(new Ant(x, y, &world)));
    }
}

void performActions(list<unique_ptr<Organism>>::iterator orgIt, list<unique_ptr<Organism>>& orgList)
{
    //Attempt to eat prey - eating prey is considered a move
    if (!((*orgIt)->eatPrey()))
    {
        //If no prey was eaten, move
        (*orgIt)->move();
    }

    //Increment the gestation and starvation counts
    (*orgIt)->incrementGestationCount();
    (*orgIt)->incrementStarvationCount();

    //Check if the organism is starving
    if (!((*orgIt)->starve()))
    {
        //If the organism did not starve, attempt to breed
        Organism* newOrganism{ (*orgIt)->breed() };

        //If the organism bred, push the new organism onto the list
        if (newOrganism != nullptr)
        {
            orgList.push_front(static_cast<unique_ptr<Organism>>(newOrganism));
        }
    }
}