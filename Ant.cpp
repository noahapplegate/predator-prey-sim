#include "Ant.h"
#include <array>
#include <algorithm>

us_int Ant::antCount{ 0 };

Ant::Ant(const us_int xPos_arg, const us_int yPos_arg, Grid * const gridPtr) :
    Organism(xPos_arg, yPos_arg, gridPtr, Species::ANT, ANT_GESTATION, ANT_STARVATION)
{
    ++antCount;
}

Ant::~Ant()
{
    --antCount;
}

Organism* Ant::clone(const us_int x, const us_int y, Grid* const gridPtr) const
{
    return new Ant(x, y, gridPtr);
}

us_int Ant::getAntCount()
{
    return antCount;
}

//Default implementation for eat prey - cannot eat prey
bool Ant::eatPrey()
{
    return false;
}

//Default implementation for starvation - cannot starve
bool Ant::starve()
{
    return false;
}

/*
Organism* Ant::breed()
{
    return Organism::breed();
}
/* DELETE THIS?
Organism* Ant::breed()
{
    //Number of turns it takes for ant to be eligible to breed
    const us_int GESTATION_PERIOD{ 3 };

    //Pointer to a new ant
    Organism* newAnt{ nullptr };

    if (getGestationCount() >= GESTATION_PERIOD)
    {
        //Randomly generate a valid breed direction
        Movements breedDirection{ generateMove(&Organism::isValidMove) };

        //Get current x and y coordinates
        const us_int xCoord{ getXPos() };
        const us_int yCoord{ getYPos() };

        //Determine which move was generated and create a new organism in that direction
        switch (breedDirection)
        {

        //Breed an organism in the position above
        case Movements::UP:
        {
            const us_int yCoordAbove = yCoord - 1;

            newAnt = new Ant(xCoord, yCoordAbove, GRID);
            resetGestationCount();
            break;
        }

        //Move the organism down
        case Movements::DOWN:
        {
            const us_int yCoordBelow = yCoord + 1;

            newAnt = new Ant(xCoord, yCoordBelow, GRID);
            resetGestationCount();
            break;
        }

        //Move the organism left
        case Movements::LEFT:
        {
            const us_int xCoordLeft = xCoord - 1;

            newAnt = new Ant(xCoordLeft, yCoord, GRID);
            resetGestationCount();
            break;
        }

        //Move the organism right
        case Movements::RIGHT:
        {
            const us_int xCoordRight = xCoord + 1;

            newAnt = new Ant(xCoordRight, yCoord, GRID);
            resetGestationCount();
            break;
        }

        //No valid moves were available
        case Movements::NONE:
        {

            break;
        }
        }
    }

    return newAnt;
}*/

bool Ant::operator<(const Organism& right)
{
    Species rightType{ right.getOrganismType() };
    bool isLessThan{ true };

    switch (rightType)
    {
    case Species::ANT:
        if (this->numMovesAvailable() < right.numMovesAvailable())
        {
            isLessThan = true;
        }
        else //if (this->numMovesAvailable() >= right.numMovesAvailable())
        {
            isLessThan = false;
        }

        break;

    case Species::DOODLEBUG:
        //Ant is considered greater than a doodlebug
        return false;
    }

    return isLessThan;
}
