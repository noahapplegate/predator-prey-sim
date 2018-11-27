#pragma once
#include "Grid.h"

class Grid;

typedef unsigned short us_int;

enum class Species { ORGANISM, DOODLEBUG, ANT };
enum class Movements { UP, DOWN, LEFT, RIGHT, NONE };

class Organism
{
private:
    us_int xPos;            //Holds the current x position of the organism on the grid
    us_int yPos;            //Holds the current y position of the organism on the grid

    bool dead;              //Flag that signals that an organism has died

    const Species organismType;         //The type of organism in the object is

    const us_int GESTATION;             //The number of turns it takes for an organism to breed
    us_int gestationCount;              //The number of turns the organism has gestated

    const us_int STARVATION_PERIOD;     //The number of turns it takes for an organism to starve
    us_int starvationCount;             //The number of turns the organism has gone without eating

protected:
    Grid* const GRID;       //Pointer to the grid on which this organism lives

public:
    Organism(const us_int xPos_arg, const us_int yPos_arg, Grid* const gridPtr, const Species orgType,
             const us_int gestation, const us_int starvation);
    virtual ~Organism();
    virtual Organism* clone(const us_int, const us_int, Grid* const) const = 0;

    us_int getXPos() const;
    us_int getYPos() const;

    void setXPos(const us_int x);
    void setYPos(const us_int y);

    bool isDead() const;
    void clearDeadFlag();
    void setDeadFlag();

    us_int getGestationCount() const;
    void resetGestationCount();
    void incrementGestationCount();

    us_int getStarvationCount() const;
    void resetStarvationCount();
    void incrementStarvationCount();

    Species getOrganismType() const;

    virtual bool move();
    virtual bool eatPrey();
    virtual Organism* breed();
    virtual bool starve();

    us_int numMovesAvailable() const;

    Movements generateMove(bool(Organism::*isActionValid)(us_int, us_int));
    virtual bool isValidMove(const us_int x, const us_int y);
    virtual bool isValidPrey(const us_int x, const us_int y);

    virtual bool operator<(const Organism& right) = 0;
};