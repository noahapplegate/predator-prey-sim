#pragma once
#include "Organism.h"
#include <memory>
#include <array>
#include <list>

class Organism;

typedef unsigned short us_int;

class Grid
{
private:
    static const us_int GRIDSIZE_MAX{ 50 };     //Maximum size of the grid's width and height
    const us_int GRIDWIDTH;                     //Size of the grid's width
    const us_int GRIDHEIGHT;                    //Size of the grid's height

    //2D Grid on which the organisms live
    std::array<std::array<Organism*, GRIDSIZE_MAX>, GRIDSIZE_MAX> organismGrid;

public:
    Grid();                                             //Sets grid width and grid height to the maximum size
    Grid(const us_int width, const us_int height);      //Sets the grid width and grid height to the specified sizes
    ~Grid();

    us_int getGridWidth() const;
    us_int getGridHeight() const;

    bool inBounds(const us_int xCoord, const us_int yCoord);
    bool availablePosition(const us_int xCoord, const us_int yCoord);
    void setGrid(const us_int xCoord, const us_int yCoord, Organism*);
    bool removeFromGrid(const us_int, const us_int);
    void displayGrid();
    const Organism* const getGridElement(const us_int, const us_int);
};

