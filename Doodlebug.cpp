#include "Doodlebug.h"

us_int Doodlebug::doodlebugCount{ 0 };

Doodlebug::Doodlebug(const us_int xPos_arg, const us_int yPos_arg, Grid * const gridPtr) :
    Organism(xPos_arg, yPos_arg, gridPtr, Species::DOODLEBUG, DOODLEBUG_GESTATION, DOODLEBUG_STARVATION)
{
    ++doodlebugCount;
}

Doodlebug::~Doodlebug()
{
    --doodlebugCount;
}

Organism* Doodlebug::clone(const us_int x, const us_int y, Grid * const gridPtr) const
{
    return new Doodlebug(x, y, gridPtr);
}

us_int Doodlebug::getDoodlebugCount()
{
    return doodlebugCount;
}

bool Doodlebug::isValidPrey(const us_int x, const us_int y)
{
    if (GRID->getGridElement(x, y) == nullptr)
    {
        return false;
    }
    else
    {
        return (GRID->getGridElement(x, y)->getOrganismType() == Species::ANT);
    }
}

bool Doodlebug::operator<(const Organism& right)
{
    Species rightType{ right.getOrganismType() };

    switch (rightType)
    {
    case Species::ANT:
        //Doodlebug is considered less than an ant
        return true;

        break;

    case Species::DOODLEBUG:
        if (this->numMovesAvailable() < right.numMovesAvailable())
        {
            return true;
        }
        else //if (this-> numMovesAvailable() >= right.numMovesAvailable())
        {
            return false;
        }

        break;

    default:
        return true;

        break;
    }
}
