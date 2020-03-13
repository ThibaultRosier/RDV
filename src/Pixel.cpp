
#include "Pixel.h"

Pixel::Pixel()
{
    r = 0;
    g = 0;
    b = 0;
}

Pixel::setRGB(unsigned int r,unsigned int g, unsigned int b)
{
    this->r = r;
    this->g = g;
    this->b = b;
}

unsigned int Pixel::getR()
{
    return r;
}

unsigned int Pixel:: getG()
{
    return g;
}

unsigned int Pixel:: getB()
{
    return b;
}


Pixel::~Pixel()
{
    //dtor
}


