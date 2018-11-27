#include "Organism.h"
#include <array>
#include <algorithm>
#include <stdexcept>

Organism* newObjectOfSameType(Organism* orgPtr, const us_int x, const us_int y, Grid* const gridPtr)
{
    return orgPtr->clone(x, y, gridPtr);
}

/// <summary>
/// The Organism constructor initializes the organism with an (x,y) position on a grid and a pointer on which the
/// organism will live. The organism is then placed on the grid. Note that an organism cannot exist without a grid.
/// </summary>
/// <param name="x"> x-coordinate where the organism will be placed on the grid </param>
/// <param name="y"> y-coordinate where the organism will be placed on the grid </param>
/// <param name="gridPtr"> Pointer to the grid on which the organism will live </param>
Organism::Organism(const us_int xPos_arg, const us_int yPos_arg, Grid* const gridPtr, const Species orgType,
    const us_int gestation, const us_int starvation) :
    //Initializer list
    xPos{ xPos_arg }, yPos{ yPos_arg }, GRID{ gridPtr }, organismType{ orgType }, GESTATION{ gestation },
    STARVATION_PERIOD{ starvation }, dead{ false }, gestationCount{ 0 }, starvationCount{ 0 }
{
    //Place the organism on the grid
    GRID->setGrid(xPos, yPos, this);
}

Organism::~Organism()
{
}

us_int Organism::getXPos() const
{
    return xPos;
}

us_int Organism::getYPos() const
{
    return yPos;
}

void Organism::setXPos(const us_int x)
{
    //Check if the specified value is a valid x position
    if (x < 0 || x >= GRID->getGridWidth())
    {
        throw std::invalid_argument("Specified x-coordinate out of grid bounds");
    }

    xPos = x;
}

void Organism::setYPos(const us_int y)
{
    //Check if the specified value is a valid y position
    if (y < 0 || y >= GRID->getGridHeight())
    {
        throw std::invalid_argument("Specified y-coordinate out of grid bounds");
    }

    yPos = y;
}

bool Organism::isDead() const
{
    return dead;
}

void Organism::clearDeadFlag()
{
    dead = false;
}

void Organism::setDeadFlag()
{
    dead = true;
}

/* DELETE THIS?
Species Organism::getOrganismType() const
{
    return Species::ORGANISM;
}*/

us_int Organism::getGestationCount() const
{
    return gestationCount;
}

void Organism::resetGestationCount()
{
    gestationCount = 0;
}

void Organism::incrementGestationCount()
{
    ++gestationCount;
}

us_int Organism::getStarvationCount() const
{
    return starvationCount;
}

void Organism::resetStarvationCount()
{
    starvationCount = 0;
}

void Organism::incrementStarvationCount()
{
    ++starvationCount;
}

Species Organism::getOrganismType() const
{
    return organismType;
}

/// <summary>
/// 
/// </summary>
bool Organism::move()
{
    if (isDead())
    {
        return false;
    }

    //Flag to signal move was made
    bool moveMade{ true };

    //Randomly generate a valid move direction
    Movements moveDirection{ generateMove(&Organism::isValidMove) };

    //Get current x and y coordinates
    const us_int xCoord{ getXPos() };
    const us_int yCoord{ getYPos() };

    //Determine which move was generated and move the organism. The organism's current position is set to nullptr, the adjacent
    //position that is moved to is set to this, and the organism's current position is updated
    switch (moveDirection)
    {

    //Move the organism up
    case Movements::UP:
    {
        const us_int yCoordAbove = yCoord - 1;

        GRID->removeFromGrid(xCoord, yCoord);
        GRID->setGrid(xCoord, yCoordAbove, this);
        setYPos(yCoordAbove);
        break;
    }

    //Move the organism down
    case Movements::DOWN:
    {
        const us_int yCoordBelow = yCoord + 1;

        GRID->removeFromGrid(xCoord, yCoord);
        GRID->setGrid(xCoord, yCoordBelow, this);
        setYPos(yCoordBelow);
        break;
    }

    //Move the organism left
    case Movements::LEFT:
    {
        const us_int xCoordLeft = xCoord - 1;

        GRID->removeFromGrid(xCoord, yCoord);
        GRID->setGrid(xCoordLeft, yCoord, this);
        setXPos(xCoordLeft);
        break;
    }

    //Move the organism right
    case Movements::RIGHT:
    {
        const us_int xCoordRight = xCoord + 1;

        GRID->removeFromGrid(xCoord, yCoord);
        GRID->setGrid(xCoordRight, yCoord, this);
        setXPos(xCoordRight);
        break;
    }

    //No valid moves were available
    case Movements::NONE:
    {
        //Signal that no move was made
        moveMade = false;

        break;
    }
    }

    return moveMade;
}

bool Organism::eatPrey()
{
    if (isDead())
    {
        return false;
    }

    //Flag to signal prey was eaten
    bool preyEaten{ true };

    //Randomly generate a valid eat direction
    Movements eatDirection{ generateMove(&Organism::isValidPrey) };

    //Get current x and y coordinates
    const us_int xCoord{ getXPos() };
    const us_int yCoord{ getYPos() };

    //Determine which move was generated and eat the organism in the generated move direction
    switch (eatDirection)
    {

    //Valid prey above
    case Movements::UP:
    {
        const us_int yCoordAbove = yCoord - 1;

        //Eat organism above
        GRID->removeFromGrid(xCoord, yCoordAbove);

        //Move this organism to the new position
        GRID->removeFromGrid(xCoord, yCoord);
        GRID->setGrid(xCoord, yCoordAbove, this);
        setYPos(yCoordAbove);

        break;
    }

    //Valid prey below
    case Movements::DOWN:
    {
        const us_int yCoordBelow = yCoord + 1;

        //Eat organism below
        GRID->removeFromGrid(xCoord, yCoordBelow);

        //Move this organism to the new position
        GRID->removeFromGrid(xCoord, yCoord);
        GRID->setGrid(xCoord, yCoordBelow, this);
        setYPos(yCoordBelow);

        break;
    }

    //Valid prey left
    case Movements::LEFT:
    {
        const us_int xCoordLeft = xCoord - 1;

        //Eat organism to the left
        GRID->removeFromGrid(xCoordLeft, yCoord);

        //Move this organism to the new position
        GRID->removeFromGrid(xCoord, yCoord);
        GRID->setGrid(xCoordLeft, yCoord, this);
        setXPos(xCoordLeft);

        break;
    }

    //Valid prey right
    case Movements::RIGHT:
    {
        const us_int xCoordRight = xCoord + 1;

        //Eat organism to the right
        GRID->removeFromGrid(xCoordRight, yCoord);

        //Move this organism to the new position
        GRID->removeFromGrid(xCoord, yCoord);
        GRID->setGrid(xCoordRight, yCoord, this);
        setXPos(xCoordRight);

        break;
    }

    //No valid prey
    case Movements::NONE:
    {
        //Signal that no prey was eaten
        preyEaten = false;

        break;
    }
    }

    //If prey was eaten, reset the starvation count
    if (preyEaten)
    {
        resetStarvationCount();
    }

    return preyEaten;
}

Organism* Organism::breed()
{
    if (isDead())
    {
        return nullptr;
    }

    //Pointer to a new organism
    Organism* newOrganism{ nullptr };

    if (getGestationCount() >= GESTATION)
    {
        //Randomly generate a valid breed direction
        Movements breedDirection{ generateMove(&Organism::isValidMove) };

        //Get current x and y coordinates
        const us_int xCoord{ getXPos() };
        const us_int yCoord{ getYPos() };

        //Determine which move was generated and create a new organism in that direction. If an organism is created, then
        //the organism's gestation count is reset
        switch (breedDirection)
        {

         //Breed an organism in the position above
        case Movements::UP:
        {
            const us_int yCoordAbove = yCoord - 1;

            newOrganism = newObjectOfSameType(this, xCoord, yCoordAbove, GRID);
            break;
        }

        //Breed an organism in the position below
        case Movements::DOWN:
        {
            const us_int yCoordBelow = yCoord + 1;

            newOrganism = newObjectOfSameType(this, xCoord, yCoordBelow, GRID);
            break;
        }

        //Breed an organism in the position to the left
        case Movements::LEFT:
        {
            const us_int xCoordLeft = xCoord - 1;

            newOrganism = newObjectOfSameType(this, xCoordLeft, yCoord, GRID);
            break;
        }

        //Breed an organism in the position to the right
        case Movements::RIGHT:
        {
            const us_int xCoordRight = xCoord + 1;

            newOrganism = newObjectOfSameType(this, xCoordRight, yCoord, GRID);
            break;
        }

        //No valid moves were available
        case Movements::NONE:
        {

            break;
        }
        }
    }

    //If a new organism was bred, reset the gestationCount
    if (newOrganism != nullptr)
    {
        resetGestationCount();
    }

    return newOrganism;
}

bool Organism::starve()
{
    if (getStarvationCount() >= STARVATION_PERIOD)
    {
        //Kill this organism
        GRID->removeFromGrid(getXPos(), getYPos());
        return true;
    }
    else
    {
        return false;
    }
}

us_int Organism::numMovesAvailable() const
{
    us_int availableMoves{ 0 };

    //Check above
    if (GRID->availablePosition(xPos, yPos - 1))
    {
        ++availableMoves;
    }

    //Check below
    if (GRID->availablePosition(xPos, yPos + 1))
    {
        ++availableMoves;
    }

    //Check left
    if (GRID->availablePosition(xPos - 1, yPos))
    {
        ++availableMoves;
    }

    //Check right
    if (GRID->availablePosition(xPos + 1, yPos))
    {
        ++availableMoves;
    }

    return availableMoves;
}

Movements Organism::generateMove(bool(Organism::*isActionValid)(us_int, us_int))
{
    //Create a random order of movements
    std::array<Movements, 4> randomMove{ Movements::UP, Movements::DOWN, Movements::LEFT, Movements::RIGHT };
    std::random_shuffle(randomMove.begin(), randomMove.end());

    //Iterate through the randomized movement order determine if the desired action is valid in that direction. Once a valid
    //direction is determined it is returned.
    size_t movesAttempted{ 0 };
    while (movesAttempted != 4)
    {
        switch (randomMove[movesAttempted])
        {
        case Movements::UP:
        {
            const us_int yPosAbove = yPos - 1;

            if ((this->*isActionValid)(xPos, yPosAbove))
            {
                return Movements::UP;
            }
            break;
        }

        case Movements::DOWN:
        {
            const us_int yPosBelow = yPos + 1;

            if ((this->*isActionValid)(xPos, yPosBelow))
            {
                return Movements::DOWN;
            }
            break;
        }

        case Movements::LEFT:
        {
            const us_int xPosLeft = xPos - 1;

            if ((this->*isActionValid)(xPosLeft, yPos))
            {
                return Movements::LEFT;
            }
            break;
        }

        case Movements::RIGHT:
        {
            const us_int xPosRight = xPos + 1;

            if ((this->*isActionValid)(xPosRight, yPos))
            {
                return Movements::RIGHT;
            }
            break;
        }
        }

        ++movesAttempted;
    }

    //If we iterate through the random list order and no movement is valid return that information
    return Movements::NONE;
}

bool Organism::isValidMove(const us_int x, const us_int y)
{
    return GRID->availablePosition(x, y);
}

bool Organism::isValidPrey(const us_int x, const us_int y)
{
    return false;
}

/* DELETE THIS?
//Doodlebugs move before ants( Doodlebug < ant )
//Ant with least number of moves < ant with more moves
bool Organism::operator<(const Organism& right)
{
    //comp(a,b) == false
    if (this == &right)
    {
        return false;
    }

    //ant < doodlebug is false
    if (this->getOrganismType() == Species::ANT && right.getOrganismType() == Species::DOODLEBUG)
    {
        return false;
    }

    //compare ant to ant
    if (this->getOrganismType() == Species::ANT && right.getOrganismType() == Species::ANT)
    {
        //more moves < less moves is false
        if (this->numMovesAvailable() >= right.numMovesAvailable())
        {
            return false;
        }
    }

    return true;
}*/