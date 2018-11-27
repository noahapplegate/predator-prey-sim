#pragma once
#include "Organism.h"

class Doodlebug : public Organism
{
private:
    static const us_int DOODLEBUG_GESTATION{ 6 };
    static const us_int DOODLEBUG_STARVATION{ 5 };
    static us_int doodlebugCount;

public:
    Doodlebug(const us_int xPos_arg, const us_int yPos_arg, Grid* const gridPtr);
    virtual ~Doodlebug();
    virtual Organism* clone(const us_int, const us_int, Grid* const gridPtr) const override;

    static us_int getDoodlebugCount();

    virtual bool isValidPrey(const us_int, const us_int) override;

    virtual bool operator<(const Organism& right);
};

