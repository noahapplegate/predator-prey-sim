#pragma once
#include "Organism.h"

class Ant : public Organism
{
private:
    static const us_int ANT_GESTATION{ 3 };
    static const us_int ANT_STARVATION{ USHRT_MAX };
    static us_int antCount;

public:
    Ant(const us_int xPos_arg, const us_int yPos_arg, Grid* const gridPtr);
    virtual ~Ant();
    virtual Organism* clone(const us_int, const us_int, Grid* const gridPtr) const override;

    static us_int getAntCount();

    virtual bool eatPrey() override;

    virtual bool starve() override;

    virtual bool operator<(const Organism& right) override;
};

