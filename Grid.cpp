#include "Grid.h"
#include <stdexcept>
#include <iostream>

/// <summary>
/// The default constructor for the Grid class. Sets the width and height of the grid to the maximum size. The entire grid
/// is set to point to nullptr.
/// </summary>
Grid::Grid() : GRIDWIDTH{ GRIDSIZE_MAX }, GRIDHEIGHT{ GRIDSIZE_MAX }
{
    for (auto row : organismGrid)
    {
        row.fill(nullptr);
    }
}

/// <summary>
/// Constuctor for the Grid class which is passed the grid dimensions. If either the width or the height is an invalid value
/// an exception is thrown.
/// </summary>
/// <param name="width"> Width of the grid </param>
/// <param name="height"> Height of the grid </param>
Grid::Grid(const us_int width, const us_int height) :
    //Initialize the width of the graph, throws an exception if an invalid width is passed
    GRIDWIDTH{ (width >= 1 && width <= GRIDSIZE_MAX) ?
    width : throw std::invalid_argument("Width must be between 1 and GRIDSIZE_MAX") },

    //Initialize the height of the graph, throws an exception if an invalid height is passed
    GRIDHEIGHT{ (height >= 1 && height <= GRIDSIZE_MAX) ?
    height : throw std::invalid_argument("Height must be between 1 and GRIDSIZE_MAX") }
{
    //Set each position on the grid to nullptr
    for (size_t row{ 0 }; row < width; ++row)
    {
        for (size_t col{ 0 }; col < height; ++col)
        {
            organismGrid.at(row).at(col) = nullptr;
        }
    }
}

Grid::~Grid()
{
}

us_int Grid::getGridWidth() const
{
    return GRIDWIDTH;
}

us_int Grid::getGridHeight() const
{
    return GRIDHEIGHT;
}

bool Grid::inBounds(const us_int xCoord, const us_int yCoord)
{
    return (xCoord >= 0 && xCoord < GRIDWIDTH) && (yCoord >= 0 && yCoord < GRIDHEIGHT);
}

/// <summary>
/// Determines if the specified (x,y) position is available on the grid. The position is considered available if it is within
/// the grid's boundaries and is not already occupied.
/// </summary>
/// <param name="x"> The x-coordinate of the position being checked </param>
/// <param name="y"> The y-coordinate of the position being checked </param>
/// <returns> True if the position is available, otherwise false </returns>
bool Grid::availablePosition(const us_int x, const us_int y)
{
    return (inBounds(x, y) && organismGrid[x][y] == nullptr) ? true : false;
}

/// <summary>
/// Adds an organism to the grid at the specified position.
/// </summary>
/// <param name="x"> x-coordinate of the position on the grid we are adding an organism to </param>
/// <param name="y"> y-coordinate of the position on the grid we are adding an organism to </param>
/// <param name="organismPtr"> Pointer to the organism we are adding to the grid </param>
void Grid::setGrid(const us_int x, const us_int y, Organism* organismPtr)
{
    //If the position is not available, then throw an exception
    if (!availablePosition(x, y))
    {
        throw std::invalid_argument("Specified position is out of grid bounds or already occupied");
    }

    //If the position is available, add the organism to the graph
    organismGrid[x][y] = organismPtr;

    //Flag that the organism is alive
    organismPtr->clearDeadFlag();
}

/// <summary>
/// Removes an organism at the specified (x,y) position if it exists.
/// </summary>
/// <param name="x"> x-coordinate of the position on the grid we are removing an organism from </param>
/// <param name="y"> y-coordinate of the position on the grid we are removing an organism from </param>
/// <returns> True if an organism is removed, false if the position was unoccupied </returns>
bool Grid::removeFromGrid(const us_int x, const us_int y)
{
    //Check the position is not empty
    if (inBounds(x, y) && organismGrid.at(x).at(y) != nullptr)
    {
        //Remove the organism from the grid
        organismGrid.at(x).at(y)->setDeadFlag();
        organismGrid.at(x).at(y) = nullptr;
        return true;
    }

    //The position was not in the grid's bounds or it was unoccupied and nothing was removed
    return false;
}

void Grid::displayGrid()
{
    for (size_t y{ 0 }; y < GRIDHEIGHT; ++y)
    {
        for (size_t x{ 0 }; x < GRIDWIDTH; ++x)
        {
            if (organismGrid.at(x).at(y) == nullptr)
            {
                std::cout << ". ";
            }
            else if (organismGrid.at(x).at(y)->getOrganismType() == Species::ANT)
            {
                std::cout << "O ";
            }
            else if (organismGrid.at(x).at(y)->getOrganismType() == Species::DOODLEBUG)
            {
                std::cout << "X ";
            }
        }

        std::cout << std::endl;
    }
}

const Organism* const Grid::getGridElement(const us_int x, const us_int y)
{
    return inBounds(x, y) ? organismGrid.at(x).at(y) : nullptr;
}
